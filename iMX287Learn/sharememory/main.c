#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "unistd.h"
#include "pthread.h"
#include "beep/threadbody.h"
#include "sharesrc/appconfig.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "string.h"
#include "sys/time.h"

pthread_t thread_body_id;//定义线程句柄(id)i
char *pShmAddr;//共享内存的首地址

char *env_init[] = {"USER=sharememory",
		    "HOME=/home/wgco/Linux_App/sharememory/loadsrc/",
		    NULL};//为子进程定义环境变量

static char *loadpath = "/home/wgco/Linux_App/sharememory/loadsrc/loadprogram";
void create_sub_process(void){
	//创建子进程，并对子进程装载新可执行程序
	pid_t pid = fork();
	if( pid < 0){//创建进程号失败
		perror("fork error");
	}else if( pid == 0 ){//fork()对子进程返回0
		execle(loadpath, "loadprogram", "I am ", "a program!", (char *)0, env_init);//子进程装载新程序
		perror("execle error");
		exit(-1);
	}
}

void create_sub_thread(void){
	int arg=1;

	int ret = pthread_create(&thread_body_id,NULL,pthread_body,(void*)arg);
	if( ret == 0 ){
		printf("@MAIN-TRACE : Success to create sub thread!\r\n");
	}else{
		perror("pthread_create error");
		exit(-1);
	}
}

void share_memory_init(void){
	int fd;
	
	//此处创建共享内存
	fd = shm_open(SHM_APP_NAME,O_CREAT|O_TRUNC|O_RDWR,S_IRUSR|S_IWUSR);
	if( fd == -1 ){
		perror("shm_open error");
		exit(-1);
	}
	
	//由于新创建的共享内存大小为0，所以要设置共享内存大小
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
	printf("@MAIN-TRACE : Current(Main) Process Pid = %d\r\n",getpid());
	
	int count=0;
	struct timeval tv;
	struct timezone tz;

 	create_sub_process();//创建子进程	
	create_sub_thread();//创建子线程
	share_memory_init();//共享内存初始化
	

	//父进程
	printf("@MAIN-TRACE : Do main thread!\r\n");
	while(1){
		gettimeofday(&tv,&tz);
		memcpy(pShmAddr,&tv,sizeof(tv));
		//memset(pShmAddr,count++,sizeof(count));//往共享内存首地址处写入一个整形的计数值
		sleep(1);//1s
	}	
}
