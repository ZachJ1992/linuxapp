#include "shmem.h" 

int app_shmem_init(SHMEM *pShm){
	pShm->pAtaddr = NULL;
	pShm->shmfd = 0;
	pShm->Create = Create;
	pShm->InitData = InitData;
	pShm->IsExist = IsExist;
	pShm->Open = Open;
	pShm->PutData = PutData;
	return 0;
}

//创建共享内存
int Create(int shmkey){
	int shmfd=shmget((key_t)shmkey,SHMSIZE,IPC_CREAT|0666)
	if(ret >= 0 ){
		printf("@SHM-TRACE : Success to craete shm! shmfd=%d\n",ret);
		return 0;
	}else{
		printf("@Debug Share memory create fail! Identifier:%d\n",ret);
		return -1;
	}
}
//初始化共享内存数据
//共享内存创建后大小是0，需要初始化其大小
int InitData(void *pAtaddr){
	if(pAtaddr != NULL){
		memset(pAtaddr,0,SHMSIZE);
	}
	return 0;
}

//判断共享内存是否存在
int IsExist(int shmkey){
	int ret;
	ret=shmget((key_t)shmkey,SHMSIZE,0666);
	if(ret >= 0){
		printf("@Debug Share memory is exist! Identifier:%d\n",ret);
		return 0;
	}else{
		printf("@Debug Share memory is not exist! Identifier:%d\n",ret);
		return -1;
	}
}
//打开共享内存
int Open(int shmkey,void *pAtaddr){
	int ret=shmget((key_t)shmkey,SHMSIZE,IPC_CREAT|0666);
	if(ret >= 0){
		sprintf("@Debug Will open share memory! Identifier:%d\n",ret);
	}else{
		sprintf("@Debug Can not open share memory! Do not exist! Identifier:%d\n",ret);
		return -1;
	}

	pAtaddr=shmat(ret,0,0);
	if(pAtaddr == (void*)-1){
		printf("@Debug Open share memory fail!\n");
		return -1;
	}else{
		printf("@Debug Open share memory success!\n");
		return 0;
	}
}
//向共享内存放置数据
int PutData(void *pAtaddr,int idx,void *ptr){
	int *p = (int*)pAtaddr;
	memcpy(&p[idx], ptr, sizeof(int));
	return 0;
}

