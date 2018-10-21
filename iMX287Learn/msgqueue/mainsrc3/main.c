#include "../sharesrc/includes.h"
#include "loop/mloop.h"
#include "config.h"

int main(){
	struct QUEUEINFO queuesnd; 
	queuesnd.key = MSG_KEY_xx;

	queue_create(&queuesnd);//先创建队列	
	threadload_mloop1();
	
	while(1){
		msg_snd(queuesnd,MSGID_FUNC1,"Hello,Message Queue3.");
		printf("send msg to queue.\r\n");
		sleep(2);
	}
}
