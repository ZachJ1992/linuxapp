#include "threadbody.h"

void *pthread_body(void *arg){
	printf("@SUBTHREAD-TRACE : Do sub thread!\r\n");
	pthread_detach(pthread_self());//分离线程
	while(1){
		//printf("@SUBTHREAD-TRACE : I am sub thread!\r\n");
		sleep(1);
	}
}
