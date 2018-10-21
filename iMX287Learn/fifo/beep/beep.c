#include "../sharesrc/includes.h"
#include "beep.h"
void *pthread_beep(void *arg);
pthread_t thread_beep_handle;

int beep_arg=1;//线程参数需要全局变量

void beep_init(void){
	int ret = pthread_create(&thread_beep_handle,NULL,pthread_beep,(void*)&beep_arg);
	if( ret == 0 ){
		printf("Success to create beep thread!\r\n");
	}else{
		ERROR_EXIT("pthread_create error");
	}
}
void *pthread_beep(void *arg){
	//void*类型的参数，应首先转化指针类型，如整型指针(int*)
	pthread_detach(pthread_self());//分离线程
	printf("Do beep thread! arg=%d\r\n",*(int*)arg);
	while(1){
		//printf("@SUBTHREAD-TRACE : I am sub thread!\r\n");
		sleep(1);
	}
}
