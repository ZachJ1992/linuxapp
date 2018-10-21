#include "../../sharesrc/includes.h"
#include "../config.h"
#include "mloop.h"
void *mainloop_thread1(void *arg);
pthread_t mainloop1_handle;
struct QUEUEINFO queuercv;

void threadload_mloop1(void){
	int res = pthread_create(&mainloop1_handle,
				NULL,
				mainloop_thread1,
				NULL);
	if( res != 0 ){
		ERROR_EXIT("error to pthread_create.");
	}
}

void app_main_init(void){
	queuercv.key = MSG_KEY_xx;
	queuercv.fd = queue_isexist(queuercv);		
}
void app_main_load(void){
}
void *mainloop_thread1(void *arg){
	pthread_detach(pthread_self());

	struct MESSAGE msg;

	app_main_init();
	app_main_load();
	
	while(1){
		msg_rcv(queuercv,&msg);
		printf("rcvmsg: type=%ld ,text=%s\r\n",msg.type,msg.text);
		usleep(500000);
	}
}

