#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "unistd.h"

char *env_init[] = {"USER=process",
		    "HOME=/home/wgco/Linux_App/process",
		    NULL};//为子进程定义环境变量

int main(int argc,char *argv[]){
	printf("Current process pid = %d\r\n",getpid());
	pid_t pid;
 
	pid = fork();
	if( pid < 0){
	//创建进程号失败
		perror("fork error");
	}else if( pid == 0 ){
	//fork()对子进程返回0
		execle("/home/wgco/Linux_App/process/loadprogram", "loadprogram", "I am ", "a program!", (char *)0, env_init);//子进程装载新程序
		perror("execle error");
		exit(-1);
	}

	//父进程
	while(1){
		printf("@Debug Parent program!\r\n");
		sleep(1);//1s
	}	

	exit(-1);
}
