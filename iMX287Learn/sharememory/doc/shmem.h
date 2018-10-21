#ifndef _SHM_H
#define _SHM_H

#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE	   (256) //共享内存大小

#define SHMID_TEST 0x0000

typedef struct _SHMEM{
	void *pAtaddr;
	int shmfd;
	int (*Create)(int shmkey);
	int (*InitData)(void *pAtaddr);
	int (*IsExist)(int shmkey);
	int (*Open)(int shmkey,void *pAtaddr);
	int (*PutData)(void *pAtaddr,int idx,void *ptr);
}SHMEM;

int app_shmem_init(SHMEM *pShm);
int Create(int shmkey);
int InitData(void *pAtaddr);
int IsExist(int shmkey);
int Open(int shmkey,void *pAtaddr);
int PutData(void *pAtaddr,int idx,void *ptr);

#endif

