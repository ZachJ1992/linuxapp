//驱动测试
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

//温度符号位
#define S 0xF800

int main(int argc,char **argv)
{
	int fd;
	int i = 0;
	unsigned char result[2];
	uint16_t data = 0;
	float temperature = 0;

	printf("will open fd..\n");
	if((fd = open("/dev/ds18b20",O_RDWR|O_NONBLOCK)) < 0 ){
        	perror("open device fail.\n");
        	return -1;
    	}else{
		printf("Open Device Ds18b20 Successful!!\n");
	}

	while(1){
		int ret;
		printf("\nWill read temperature...\n");
        	usleep(100);

        	ret = read(fd, result, sizeof(result));
		if(ret < 0){
			printf("read wrong\n");
			exit(0);
		}else{
			printf("read success! %x,%x\n",result[0],result[1]);
		}

		data = result[1]; 
        	data <<=8;
        	data = data | result[0];

		if((data&S) == 0){
        		temperature =data * 0.0625 ;
        		printf("Temperature = %.2f ℃ \n",temperature);
		}else if((data&S) == S){
			data = ~data; //按位取反
			data += 1;    //再加1
        		temperature =data * 0.0625 ;
        		printf("Temperature = -%.2f ℃ \n",temperature);
		}else{
			printf("error: 'sign' bit \n");
		}
        	fflush(stdout);
        	sleep(1);
	}
	close(fd);
	return 0;
}
