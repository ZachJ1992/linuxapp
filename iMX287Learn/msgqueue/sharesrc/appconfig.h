#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

#define ERROR_EXIT(M) \
	do{ \
		perror(M); \
		exit(EXIT_FAILURE); \
	}while(0) 

//POSIX共享内存
#define SHM_APP_NAME "shmtest"
#define SHM_APP_SIZE 256 //字节个数

//有名管道文件（路径）名
#define FIFO_APP_NAME "/tmp/fifo"

#endif

