#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

main(){
	int fd,i,COUNT=4;
	char* leds_node="/dev/ledstest_ctl";
	
	if((fd = open(leds_node,O_RDWR|O_NDELAY))<0){
		printf("APP open %s failed",leds_node);
		return;
	}
	printf("APP open %s success",leds_node);
	
	for(i=0;i<COUNT;i++){
		ioctl(fd,1,1);
		sleep(3);
		ioctl(fd,0,1);
		sleep(3);
	}
	ioctl(fd,1,1);
	
	close(fd);
}

//arm-none-linux-gnueabi-gcc -o b_invokeLeds invoke_leds.c -static
