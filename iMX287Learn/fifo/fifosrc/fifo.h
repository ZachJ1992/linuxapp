#ifndef _FIFO_H_
#define _FIFO_H_

int fifo_create(const char *pname);
int fifo_write_open(const char *pname);
int fifo_read_open(const char *pname);
int fifo_write(int fd,char *pbuff,unsigned int);
int fifo_read(int fd,char *pbuff,unsigned int);

#endif

