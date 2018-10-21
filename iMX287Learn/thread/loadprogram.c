#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

extern char **environ;
int main(int argc,char *argv[]){
	int i;
	printf("@SUBPROCESS-TRACE : argc=%d\r\n",argc);
	printf("@SUBPROCESS-TRACE : args:");
	for(i=0;i<argc;i++){
		printf(" %s",argv[i]);
	}
	printf("\r\n");

	i=0;
	while(environ[i]){
		puts(environ[i++]);
	}
	printf("\r\n");
	
	printf("@SUBPEOCESS-TRACE : Do main thraed!\r\n");
	while(1){
		sleep(1);
	}
	return 0;
}
