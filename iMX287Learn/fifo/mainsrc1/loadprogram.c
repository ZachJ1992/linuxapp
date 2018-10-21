#include "../sharesrc/includes.h"

extern char **environ;
char *pShmAddr;
void share_memory_init(void){
	int fd;
	
	//此处打开共享内存
	fd = shm_open(SHM_APP_NAME,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	if( fd == -1 ){
		perror("shm_open error");
		exit(-1);
	}
	
	//设置共享内存大小
	if( ftruncate(fd,SHM_APP_SIZE) == -1 ){
		perror("ftruncate error");
		exit(-1);
	}
	//将共享内存首地址映射到此应用进程
	pShmAddr = mmap(NULL,SHM_APP_SIZE, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if( pShmAddr == MAP_FAILED){
		perror("mmap error");
		exit(-1);
	}
	//此处保留开辟的共享内存，异常退出删除共享内存有待实现
	//取消映射，删除共享内存，将不保留数据
	/*
	if( munmap(pShmAddr,SHM_APP_SIZE) == -1 ){
		perror("munmap error");
	}	
	if( shm_unlink(SHM_APP_NAME) == -1 ){
		perror("shm_unlink error");
	}*/	
}
int main(int argc,char *argv[]){
	int i;
	struct timeval tv;
	printf("@SUBPROCESS-TRACE : argc=%d\r\n",argc);
	printf("@SUBPROCESS-TRACE : args:");
	for(i=0;i<argc;i++){
		printf(" %s",argv[i]);
	}
	printf("\r\n");

	i=0;
	while(environ[i]){
		puts(environ[i++]);
	}
	printf("\r\n");
	
	//初始化
	share_memory_init();

	//读管道线程
	rfifo_init();

	printf("@SUBPEOCESS-TRACE : Do main thraed!\r\n");
	while(1){
		//memcpy(&tv,pShmAddr,sizeof(tv));
		//printf("@SUBPROCESS-TRACE : tv_sec=%ld, tv_usec=%ld\r\n",tv.tv_sec,tv.tv_usec);
		//printf("@SUBPROCESS-TRACE : read shm data=%d\r\n",*pShmAddr);
		sleep(1);
	}
	return 0;
}
