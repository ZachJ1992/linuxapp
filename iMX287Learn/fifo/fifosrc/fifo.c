#include "../sharesrc/includes.h"
/*函数功能：管道的创建
  函数参数：pname路径及名字
  返回值：成功则返回0
*/
int fifo_create(const char *pname){
	int res = access(pname,F_OK);
	if( res == 0 ){//文件存在，则删除	
		printf("fifo has existed! To rm it...\r\n");
		system("rm /tmp/fifo");
		//execlp("rm","-f",pname,NULL);
	}
	res = mkfifo(pname,O_CREAT);
	if( res < 0 ){
		ERROR_EXIT("error to mkfifo");
	}
	printf("Sucess to mkfifo.\r\n");
	return 0;
}
/*函数功能：只写方式打开fifo
  函数参数：pname路径及名字
  返回值 ：成功则返回打开的文件描述符
*/
int fifo_write_open(const char *pname){
	int res = open(pname,O_WRONLY,0);
	if( res < 0 ){
		ERROR_EXIT("error to open write fifo");
	}
	printf("Success to open write fifo.\r\n");
	return res;
}
/*函数功能：只读方式打开fifo
  函数参数：pname路径及名字
  返回值 ：成功则返回打开的文件描述符
*/
int fifo_read_open(const char *pname){
	int res = open(pname,O_RDONLY,0);
	if( res < 0 ){
		ERROR_EXIT("error to open read fifo");
	}
	printf("Success to open read fifo.\r\n");
	return res;
}
//向fifo写数据
int fifo_write(int fd,char *pbuff,unsigned int len){
	int res = write(fd,pbuff,len);
	if( res < 0 ){
		ERROR_EXIT("error to write fifo");
	}
	return res;
}
//从fifo读数据
int fifo_read(int fd,char *pbuff,unsigned int len){
	int res = read(fd,pbuff,len);
	if( res < 0 ){
		ERROR_EXIT("error to read fifo");
	}
	return res;
}
int fifo_delete(int fd){
}
