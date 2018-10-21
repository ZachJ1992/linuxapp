#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
void process_client_handle(int s){
    ssize_t size = 0;
    char buffer[1024];
    for(;;){
        //从标准输入中读取数据放到缓冲区buffer中
        size = read(0,buffer,1024);
        if( size > 0 ){ //读到数据
            write(s,buffer,size); //发送给服务器
            size = read(s,buffer,1024); //从服务器读取数据
            write(1,buffer,size);
        }
    }
}
int main(int argc,char *argv[]){
    int s; //s为socket描述符
    struct sockaddr_in server_addr; //服务器地址结构
    int err; //返回值

    s = socket(AF_INET,SOCK_STREAM,0); //建立一个流式套结字
    if( s < 0 ){
        printf("socket error \n");
        return -1;
    }
    
    //设置服务器地址
    bzero(&server_addr,sizeof(server_addr)); //清零
    server_addr.sin_family = AF_INET; //协议族
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //本地协议
    server_addr.sin_port = htons(PORT); //服务器端口

    //将用户的字符串类型的IP地址转化为整形
    inet_pton(AF_INET,argv[1],&server_addr.sin_addr);
    //连接服务器
    connect(s,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
    process_client_handle(s); //客户端处理过程  
    close(s);
}


