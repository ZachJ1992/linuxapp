#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

main(){
	int fd;
	char* wgcotest_node="/dev/deviceNodeReg_dev";	

	if((fd=open(wgcotest_node,O_RDWR|O_NDELAY))<0){
		printf("APP open %s failed\n",wgcotest_node);
	}else{
		printf("APP open %s success\n",wgcotest_node);
		ioctl(fd,1,6);
	}
	close(fd);
}

// arm-none-linux-gnueabi-gcc -o b_invokeApp invokeApp.c -static
//执行编译好的b_invokeApp前，应该首先加载设备节点insmod devNodeReg.ko，在/dev中可以看到加载的设备节点deviceNodeReg_dev，然后获取最高权限chmod 777 b_invokeApp并执行./b_invokeApp
