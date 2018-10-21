#include "../sharesrc/includes.h"
#include "wfifo.h"

void *pthread_wfifo(void*);
const char *fifoname = FIFO_APP_NAME;
pthread_t thread_wfifo_handle;
int wfifo_arg=2;//简单的线程参数
static int fifofd;
char buff[1024];	
char *str="Hello,welcome to linux world!";

void wfifo_init(void){
	//先于线程创建fifo，防止未被创建而去打开的情况
	fifo_create(fifoname);

	//线程创建
	if( pthread_create(&thread_wfifo_handle,NULL,pthread_wfifo,(void*)&wfifo_arg) == 0 ){
		printf("Success to craete wfifo thraed!\r\n");
	}else{
		ERROR_EXIT("pthread_create");
	}
}
void *pthread_wfifo(void* arg){	
	pthread_detach(pthread_self());//分离线程

	fifofd = fifo_write_open(fifoname);

	while(1){
		fifo_write(fifofd,str,strlen(str));
		sleep(1);
		//close(fifofd);
		//close(datafd);
	}
}
