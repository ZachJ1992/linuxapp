#ifndef _COMM_SUB_H_
#define _COMM_SUB_H_
#include "stdint.h"

#define COMM_SUB_PORT 1
#define MBUS_BUF_SIZE 128
#define MBUS_RCV_MS   150

struct MBUSPACKET{
    uint8_t addr;
    uint8_t ins;
    uint8_t Reg[2];
    uint8_t Data[2];
    uint8_t Crc[2];
};

//写
typedef struct MBUSPACKET   MBUS_WRITE_SND;
typedef struct MBUSPACKET   MBUS_WRITE_RSP;

//读
typedef struct MBUSPACKET   MBUS_READ_SND;
    
struct MBUS_READ_RSP{
    uint8_t addr;
    uint8_t ins;
    uint8_t bytes; //读取的字节数量
    uint8_t Data[MBUS_BUF_SIZE-3];
};

#define COMM_SUB_LIST_MAX 10
struct COMMSUBLIST{
    uint8_t rev0;
    uint8_t index;
    uint8_t eof;
    uint8_t sof;
    MBUS_WRITE_SND MBus[COMM_SUB_LIST_MAX];
};

#define CHECK_PASS          0x00
#define CHECK_PACKET_ERROR  0x01
#define CHECK_CRCBUF_EQUAL  0x02

void comm_sub_start_th(void);
void comm_sub_start_rcv(int ms);

#endif

