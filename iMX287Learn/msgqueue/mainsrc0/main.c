#include "../sharesrc/includes.h"

#define MSGID_TEST 1

int main(){
	struct QUEUEINFO testqueue; 

	queue_create(&testqueue);	

	while(1){
		msg_snd(testqueue,MSGID_TEST,"Hello,Message Queue.");
		printf("send msg to queue.\r\n");
		sleep(2);
	}
}
