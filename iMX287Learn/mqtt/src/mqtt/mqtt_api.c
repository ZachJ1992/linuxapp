#include "stdio.h"
#include "errno.h"
#include "unistd.h"
#include "stdint.h"
#include "stdbool.h"
#include "pthread.h"
#include "strings.h"
#include <netinet/in.h>
#include "arpa/inet.h"
#include "netdb.h"
#include "string.h"
#include "stdlib.h"
#include "sys/epoll.h"
#include "fcntl.h"

#include "mqtt_api.h"
#include "sdk/mqtt.h"


char* prjid = "124337"; //project_id
char* auth_info = "MyMqttTest0120180320";
char* devid = "26557054";  //device_id

struct MQTT{
    int fd;
    struct MqttContext ctx[1];
    struct MqttBuffer buf[1];
    
    const char* host;
    unsigned short port;
    int epfd;
};

static pthread_t _connect_handle[1];
struct MQTT _Mqtt;

int drv_mqtt_sendPacket(void* arg,const struct iovec* iov,int iovcnt){
    int bytes;
    struct msghdr msg;
    memset(&msg,0,sizeof(msg));
    msg.msg_iov = (struct iovec*)iov;
    msg.msg_iovlen = (size_t)iovcnt;

    int i=0,j=0;
    printf("send one pkt\n");
    for(i=0;i<iovcnt;++i){
        char* pkg = (char*)iov[i].iov_base;
        if( iov[i].iov_len > 1024 ){
            printf("length:%d\n", (int)iov[i].iov_len);
        }else{
            for(j=0;j<iov[i].iov_len;++j){
                printf("%02X ", pkg[j]&0xFF);
            }
            printf("\n");
        }
    }
    printf("send over\n");
    bytes = sendmsg((int)(size_t)arg,&msg,0);
    return bytes;
}
int drv_mqtt_recivePacket(void* arg,void* buf,uint32_t count){
    int bytes = read((int)(size_t)arg,buf,count);
    return bytes;
}
int drv_mqtt_connackHandle(void* arg,char flags,char ret_code){
    printf("Success to connect to the server,flags(%0x),code(%d).\n",flags,ret_code);
    return 0;
}
int drv_mqtt_createTcpConnect(const char* host,unsigned short port){
    struct sockaddr_in addr;
    int fd;
    struct hostent* pServer;

    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); 
    pServer = gethostbyname(host);
    if( pServer == NULL ){
        printf("Failed to get the ip of the host(%s).\n", host);
        return -1;
    }

    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0) {
        printf("Failed to create socket file descriptor.\n");
        return fd;
    }

    bcopy((char*)pServer->h_addr,(char*)&addr.sin_addr.s_addr,pServer->h_length);
    if( connect(fd,(struct sockaddr*)&addr,sizeof(addr)) == -1 ){
        printf("Failed to connect to the server.\n");
        close(fd);
        return -1;
    }

    return fd;
}
int drv_mqtt_connectCmd(struct MQTT* pMqtt){
    struct epoll_event event;
    int flags;

    if( pMqtt->fd >= 0 ){
        close(pMqtt->fd);
        epoll_ctl(pMqtt->epfd,EPOLL_CTL_DEL,pMqtt->fd,NULL);
    }

    pMqtt->fd = drv_mqtt_createTcpConnect(pMqtt->host,pMqtt->port);
    if( pMqtt->fd < 0 ){
        return -1;
    }

    pMqtt->ctx->read_func_arg = (void*)(size_t)pMqtt->fd;
    pMqtt->ctx->writev_func_arg = (void*)(size_t)pMqtt->fd;

    //设置socket为非阻塞
    //1.先获取socket的flags
    flags = fcntl(pMqtt->fd,F_GETFL,0);
    if( flags == -1 ){
        printf("Failed to get the socket file flags, errcode is %d.\n", errno);
    }
    //2.再设置非阻塞标志
    if( fcntl(pMqtt->fd,F_SETFL,flags | O_NONBLOCK) < 0 ){
        printf("Failed to set the socket to nonblock mode, errcode is %d.\n", errno);
        return -1;
    }

    //添加socket连接的输入事件到epoll监听事件
    event.data.fd = pMqtt->fd;
    event.events = EPOLLIN | EPOLLONESHOT | EPOLLET;
    if( epoll_ctl(pMqtt->epfd,EPOLL_CTL_ADD,pMqtt->fd,&event) < 0 ){
        printf("Failed to add the socket to the epoll, errcode is %d.\n", errno);
        return -1;
    }

    int keep_alive = 1200;
    printf("dev id:%s\n", devid );
    printf("project id:%s\n", prjid);
    printf("auth info:%s\n", auth_info);

    int err;
    err = Mqtt_PackConnectPkt(pMqtt->buf,keep_alive,devid,
                             1,NULL,
                             NULL,0,
                             MQTT_QOS_LEVEL0,0,prjid,
                             auth_info,strlen(auth_info));
    if( err != MQTTERR_NOERROR ){
        printf("Failed to pack the MQTT CONNECT PACKET, errcode is %d.\n", err);
        return -1;
    }

    int bytes;
    bytes = Mqtt_SendPkt(pMqtt->ctx,pMqtt->buf,0);
    if( bytes < 0 ){
        printf("Failed to send the packet to the server.\n");
        return -1;
    }

    MqttBuffer_Reset(pMqtt->buf);
    return 0;
}

int drv_mqtt_socketHandle(struct MQTT* pMqtt,uint32_t events){
    struct epoll_event evt[1];
    evt->data.fd = pMqtt->fd;
    evt->events = EPOLLIN;
    
    if( events & EPOLLIN){
        while(1){
            int err;
            err = Mqtt_RecvPkt(pMqtt->ctx);
            if( err == MQTTERR_ENDOFFILE ){
                printf("The connect is disconnect.\n");
                close(pMqtt->fd);
                epoll_ctl(pMqtt->epfd,EPOLL_CTL_DEL,pMqtt->fd,NULL);
                pMqtt->fd = -1;
                return 0;
            }
            if( err == MQTTERR_IO ){
                if( (errno==EAGAIN) || (errno==EWOULDBLOCK) ){
                    break;
                }
                printf("Send TCP data error: %s.\n", strerror(errno));
                return -1;
            }
            if( err != MQTTERR_NOERROR ){
                printf("Mqtt_RecvPkt error is %d.\n", err);
                return -1;
            }
        }
    }
    epoll_ctl(pMqtt->epfd,EPOLL_CTL_MOD,pMqtt->fd,evt);
    return 0;
}

