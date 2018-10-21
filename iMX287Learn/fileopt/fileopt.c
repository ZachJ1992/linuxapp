#include "sharesrc/includes.h"

int main(){
	int fd;
	char *str = "Hello,linux";
	char buff[1024];
	
	fd = open("data.txt",O_WRONLY|O_CREAT,0777);
	if( fd < 0){
		ERROR_EXIT("open file error");
	}
	
	int bytes = write(fd,str,strlen(str));
	if( bytes < 0 ){
		ERROR_EXIT("write error");
	}
	
	int ret=-1;
	fd = fsync(fd);
	if( fd < 0 ){
		ERROR_EXIT("fsync error");
	}	
	ret = close(fd);
	if( fd < 0 ){
		ERROR_EXIT("close error");
	}
	
	////////////
	fd = open("data.txt",O_RDONLY,0777);
	if( fd < 0){
		ERROR_EXIT("open file error");
	}
	
	memset(buff,0,sizeof(buff));
	bytes = 0;
	bytes = read(fd,buff,1024);
	if( bytes < 0 ){
		ERROR_EXIT("write error");
	}else{
		printf("raed %d bytes. buff=%s\r\n",bytes,buff);
	}
	ret = close(fd);
	if( fd < 0 ){
		ERROR_EXIT("close error");
	}
	
	return 0;
}
