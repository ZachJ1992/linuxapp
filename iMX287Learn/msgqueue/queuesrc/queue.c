#include "../sharesrc/includes.h"

//创建消息队列,成功返回队列ID
int queue_isexist(struct QUEUEINFO qinfo){
	int res = msgget(qinfo.key,IPC_EXCL);
	if( res < 0 ){
		printf("queue not existed.\r\n");
		return -1;
	}
	printf("queue(fd=%d) is existed.\r\n",res);
	return res;
}
int queue_create(struct QUEUEINFO *pinfo){ 
	int res = queue_isexist(*pinfo);
	if( res != -1 ){
		//队列存在
		pinfo->fd = res;
		return 0;
	}

	//pinfo->key = MSGKEY;//ftok(QUEUEPATH,(int)"a");//根据不同的路径和关键表示产生标准的 key
	if( pinfo->key < 0 ){
		ERROR_EXIT("error to ftok a key");
	} 
	printf("Success to ftok a mq key. key=%d\r\n",pinfo->key);
	pinfo->fd = msgget(pinfo->key,IPC_CREAT|0666);
	if( pinfo->fd < 0 ){
		ERROR_EXIT("error to msgget");
	}
	printf("Success to msgget. queueid=%d\r\n",pinfo->fd);
	return 0;
}
//发送消息
//参数：qid是队列id；msgid是消息id；pbuff
int msg_snd(struct QUEUEINFO qinfo,unsigned int msgid,void *pbuff){
	struct MESSAGE msg;
	memset(&msg,0,sizeof(struct MESSAGE));
	msg.type = msgid;
	memcpy(msg.text,pbuff,strlen((char*)pbuff));
	int res = msgsnd(qinfo.fd,&msg,4+strlen(msg.text),0);//阻塞
	if( res < 0 ){
		ERROR_EXIT("error to msgsnd");
	}
	return 0;
}
//接受消息队列
ssize_t msg_rcv(struct QUEUEINFO qinfo,struct MESSAGE *pmsg){
	memset(pmsg,0,sizeof(struct MESSAGE));
	ssize_t bytes = msgrcv(qinfo.fd,pmsg,sizeof(struct MESSAGE),0,0);
	if( bytes < 0 ){
		ERROR_EXIT("error to msgrcv");
	}
	return bytes;
}

