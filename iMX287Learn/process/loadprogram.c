#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

extern char **environ;
int main(int argc,char *argv[]){
	int i;
	printf("argc=%d\r\n",argc);
	printf("args:");
	for(i=0;i<argc;i++){
		printf(" %s",argv[i]);
	}
	printf("\r\n");

	i=0;
	while(environ[i]){
		puts(environ[i++]);
	}
	printf("\r\n");

	while(1){
		printf("Child process!\r\n");
		sleep(1);
	}
	return 0;
}
