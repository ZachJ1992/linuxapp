#include "../sharesrc/includes.h"

int main(){
	struct QUEUEINFO testqueue;
	struct MESSAGE msg;
	
	testqueue.key = MSGKEY;
	testqueue.fd = queue_isexist(testqueue);

	while(1){
		msg_rcv(testqueue,&msg);
		printf("recive msg: msg.type=%ld, msg.text=%s\r\n",msg.type,msg.text);
		usleep(500000);
	}
}
