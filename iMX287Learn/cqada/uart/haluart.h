#ifndef _HALUART_H_
#define _HALUART_H_
#include "stdint.h"

int uart_open_port(int port);
int uart_set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop);

void uart_sendbyte(int fd,const uint8_t byte);
void uart_sendbytes(int fd,uint8_t* pBuf,size_t len);
uint8_t uart_readbyte(int fd);

#endif

