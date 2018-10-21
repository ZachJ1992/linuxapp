#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8888   //侦听端口地址
#define BACKLOG 2   //侦听队列长度

void process_client_handle(int s){ //子进程，服务器对客户端的处理
    ssize_t size = 0;
    char buffer[1024];
    
    for(;;){
        size = read(s,buffer,1024); //从套结字中读取数据放到缓冲区buffer中
        
        if( size == 0 ){
            return;
        }
        
        //构建响应字符，为接收到客户端字节的数量
        printf("recive from client : %s\n",buffer);
        sprintf(buffer,"%ld bytes altogether\n",size);
        write(s,buffer,strlen(buffer)+1); //发送给客户端
        bzero(buffer,sizeof(buffer)); 
    }
}
int main(int argc,char *argv[]){
    int ss,sc; //ss为服务器的socket描述符，sc为客户端的socket描述符 
    struct sockaddr_in server_addr; //服务器地址结构
    struct sockaddr_in client_addr; //客户端地址结构
    int err; //返回值
    pid_t pid; //分叉的进程ID
    
    //建立一个流式套结字
    ss = socket(AF_INET,SOCK_STREAM,0);
    if( ss < 0 ){
        printf("socket error\n");
        return -1;
    }
    
    //设置服务器地址
    bzero(&server_addr,sizeof(server_addr)); //清零
    server_addr.sin_family = AF_INET;   //协议族
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //本地地址
    server_addr.sin_port = htons(PORT); //服务器端口

    //绑定地址结构到套结字描述符
    err = bind(ss,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if( err < 0 ){
        printf("bind error\n");
        return -1;
    }
    printf("bind success\n");
    
    //设置侦听，将套结字的侦听队列长度设置为2，可以同时处理两个客户端的连接请求
    err = listen(ss,BACKLOG);
    if( err < 0 ){
        printf("listen error\n");
        return -1;
    }
    printf("listen success\n");

    //主循环过程
    for(;;){
        int addrlen = sizeof(struct sockaddr);
        
        //如果有客户端建立连接，接收客户端连接，则会返回一个新的连接描述符sc
        sc = accept(ss,(struct sockaddr*)&client_addr,&addrlen);
        if( sc < 0 ){
            continue; //结束本次循环
        }
        printf("accept sc=%d\n",sc);

        //建立一个新的进程处理到来的连接
        pid = fork(); //分叉进程
        if( pid == 0 ){ //子进程中
            printf("in sub process\n");
            close(ss); //在子进程中关闭服务器的侦听，并不会关闭父进程的套结字，子进程依然可以接收客户端的数据
            process_client_handle(sc); //子进程，服务器对客户端的处理
        }else{
            printf("in main process\n");
            close(sc); //在父进程中关闭客户端的连接
        }
    }
}
