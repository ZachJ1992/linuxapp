#include "../includes/includes.h"
#include "../timer/gettime.h"
#include "../driver/config.h"
#include "udpser.h"

extern int fns_cardet_getflows(int);

static struct timeval _fnstick;
static int _sockfd;
struct sockaddr_in _seraddr;  //服务端地址
struct sockaddr_in _destaddr; //目的地址

void fns_udp_init(void){
    int ret;    

    //创建socket描述符
    _sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if( _sockfd < 0 ){
        perror("socket() error");
        exit(1);
    }
    
    //配置为广播模式   
    const int opt = 1;
    ret =  setsockopt(_sockfd,SOL_SOCKET,SO_BROADCAST,(const char*)&opt,sizeof(opt));
    if( ret < 0 ){
        perror("setsockopt() error");
        exit(1);
    }
    
    //配置地址
    (void)memset(&_seraddr,0,sizeof(struct sockaddr_in));
    _seraddr.sin_family      = AF_INET;
    _seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _seraddr.sin_port        = htons(UDPSER_PORT); 

    //绑定
    ret = bind(_sockfd,(struct sockaddr*)&_seraddr,sizeof(struct sockaddr));    
    if( ret < 0 ){
        perror("bind() error");
        exit(1);
    }

    //设置目的地址
    (void)memset(&_destaddr,0,sizeof(struct sockaddr_in));
    _destaddr.sin_family      = AF_INET;
    _destaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    _destaddr.sin_port        = htons(UDPDEST_PORT);
}
void fns_udp_load(void){

}

void fns_udp_sndonce(char* buf){
    int ret;
    
    ret = sendto(_sockfd,buf,strlen(buf),0,(struct sockaddr*)&_destaddr,sizeof(struct sockaddr_in));
    if( ret < 1 ){
        perror("sendto() error");
        exit(1);
    }
}

void fns_udp_loop(void){
    int i;
    char buf[1024]={0};

    for(i=0;i<CHANNEL_MAX;i++){
        int f = fns_cardet_getflows(i);
        sprintf(buf+(i*20),"ch:%04d,flow(s):%04d",i,f);
    }
    fns_udp_sndonce(buf);
}
void app_udp_loop(void){
    if( timercmp(get_timeval_now(NULL),&_fnstick,>) ){
        fns_udp_loop();
        timeradd(get_timeval_now(NULL),get_timeval_ms(1000),&_fnstick);
    }
}


