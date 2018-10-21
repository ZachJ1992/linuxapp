#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "unistd.h"
#include "pthread.h"
#include "threadbody.h"

pthread_t thread_body_id;//定义线程句柄(id)i
char *env_init[] = {"USER=process",
		    "HOME=/home/wgco/Linux_App/thread",
		    NULL};//为子进程定义环境变量

void create_sub_process(void){
	//创建子进程，并对子进程装载新可执行程序
	pid_t pid = fork();
	if( pid < 0){//创建进程号失败
		perror("fork error");
	}else if( pid == 0 ){//fork()对子进程返回0
		execle("/home/wgco/Linux_App/thread/loadprogram", "loadprogram", "I am ", "a program!", (char *)0, env_init);//子进程装载新程序
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
		printf("@MAIN-DEBUG : Fail to create sub thread!\r\n");
		exit(-1);
	}
} 
int main(int argc,char *argv[]){
	printf("@MAIN-TRACE : Current(Main) Process Pid = %d\r\n",getpid());
 	create_sub_process();//创建子进程	
	create_sub_thread();//创建子线程

	//父进程
	printf("@MAIN-TRACE : Do main thread!\r\n");
	while(1){
		sleep(1);//1s
	}	
}
