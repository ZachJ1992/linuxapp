#include "stdio.h"

#include "mqtt_app.h"

static uint8_t _connect_state;

int fns_mqtt__init(struct MQTT* pMqtt){
    pMqtt->fd = -1;
    pMqtt->host = "183.230.40.39";
    pMqtt->port = 6002;

    int err;
    err = Mqtt_InitContext(pMqtt->ctx,1 << 20);
    if( MQTTERR_NOERROR != err ){
        perror("err:Mqtt_InitContext");
        exit(-1);
    }
    
    pMqtt->ctx->read_func = mqtt_recive_packet;
    pMqtt->ctx->read_func_arg = (void*)(size_t)pMqtt->fd;
    pMqtt->ctx->writev_func = mqtt_send_packet;
    pMqtt->ctx->writev_func_arg = (void*)(size_t)pMqtt->fd;

    //打印字符提示连接服务器成功回调函数
    pMqtt->ctx->handle_conn_ack = mqtt_connack_handle;
    pMqtt->ctx->handle_conn_ack_arg = pMqtt;
    
    //...

    MqttBuffer_Init(pMqtt->buf);

    //epoll
    pMqtt->epfd = epoll_create(10);
    if( pMqtt->epfd < 0 ){
        printf("Failed to create the epoll instance.\n");
        exit(-1);
    }
    
    //发送连接Onenet平台指令
    if( mqtt_connect_cmd(pMqtt) < 0 ){
        perror("err:mqtt connect");
        exit(-1);
    }
}
void fns_mqtt_doEpollEvents(void* arg){
    int evt_cnt;
    const int evt_max_cnt = 2;
    struct epoll_event events[evt_max_cnt];
    struct MQTT* pMqtt = (struct MQTT*)arg;
    while( (evt_cnt = epoll_wait(pMqtt->epfd,events,evt_max_cnt,-1)) >= 0 ){
        int i;
        for(i=0;i<evt_cnt;++i){
            if( events[i].data.fd == pMqtt->fd ){
                printf("Event for pMqtt->fd.\n");
                if( mqtt_socket_handle(pMqtt,events[i].events) < 0 ){
                    break;
                } 
            }
        }
    }
}
void fns_mqtt_loop(void* arg){
    fns_mqtt_doEpollEvents(arg);
}

void *fns_mqtt_thread(void* arg){
    pthread_detach(pthread_self());
    fns_mqtt_init(&_Mqtt);
    while(1){
        fns_mqtt_loop(&_Mqtt);
    }
}

void fns_mqtt_start_th(void){
    _connect_state = CONNECT_NONE;
    int ret;
    if( (ret = pthread_create(&_connect_handle[0],NULL,connect_thread,NULL)) != 0 ){
        perror("err:pthread_create");
        exit(-1);
    }
}
bool isMqttConnec(void){
    if( _connect_state == CONNECT_OK ){
        return true;
    }
    return false;
}
