#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

#define ERROR_EXIT(M) \
	do{ \
		perror(M); \
		exit(EXIT_FAILURE); \
	}while(0) 
#endif

