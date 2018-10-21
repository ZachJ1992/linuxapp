#ifndef _QUEUE_H_
#define _QUEUE_H_

#define MSGKEY 183515
#define QUEUEPATH "/home/wgco/linuxapp/msgqueue/mainsrc0/main.c"

struct QUEUEINFO{
	key_t key;
	int fd;
};

struct MESSAGE{
	long type;
	char text[512];
};

int queue_isexist(struct QUEUEINFO qinfo);
int queue_create(struct QUEUEINFO *pinfo);
int msg_snd(struct QUEUEINFO qinfo,unsigned int msgid,void *pbuff);
ssize_t msg_rcv(struct QUEUEINFO qinfo,struct MESSAGE *pmsg);

#endif

