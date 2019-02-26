#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

int fd;
uint32_t *pmap;
uint8_t  val=0;

int main(int argc,char *argv[])
{
	printf("test app start...\n");
	if((fd=open("/dev/userio",O_RDWR))<0){
		perror("open");
		exit(0);	
	}
	/* 暂时不使用将地址映射到用户空间的方式
	if((pmap=(uint32_t *)mmap(0,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED){
		perror("mmap");
		exit(0);
	}
	*/

	while(1){
		printf("write %d to IO1_26\n",val);
		if(write(fd,&val,1) < 0){
			perror("write");
			exit(0);
		}
		
		if(val){
			val = 0;
		}else{
			val = 1;
		}
		sleep(1);
	}	
	
	return 0;
}

