#include "../sharesrc/includes.h"
#include "rfifo.h"

void *pthread_rfifo(void*);
const char *fifoname = FIFO_APP_NAME;
pthread_t thread_rfifo_handle;
int rfifo_arg=3;//简单的线程参数
static int fifofd;

void rfifo_init(void){
	//close(fifofd);
	//close(datafd);

	//线程创建
	if( pthread_create(&thread_rfifo_handle,NULL,pthread_rfifo,(void*)&rfifo_arg) == 0 ){
		printf("Success to craete rfifo thraed!\r\n");
	}else{
		ERROR_EXIT("pthread_create");
	}	
}
void *pthread_rfifo(void* arg){	
	pthread_detach(pthread_self());//分离线程

	int bytes=0;
	char buff[1024];

	fifofd = fifo_read_open(fifoname);
		
	while(1){
		memset(buff,'\0',sizeof(buff));
		bytes = fifo_read(fifofd,buff,sizeof(buff));	
		if( bytes > 0 ){
			printf("read pipe : %s\r\n",buff);
		}
	}
}
