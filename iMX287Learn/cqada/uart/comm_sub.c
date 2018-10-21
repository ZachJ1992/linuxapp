#include "stdio.h"
#include "pthread.h"
#include "unistd.h"
#include "stdlib.h"
#include "comm_sub.h"
#include "haluart.h"
#include "strings.h"
#include "string.h"

#include "../share/usrtypes.h"
#include "../share/usrtime.h"
#include "../share/mbuscrc.h"

static pthread_t _comm_sub_handle[1];
static int _fd_uart;
static volatile COMMRCV _rcv;
static gint64_t _time_out = 0;
static struct COMMSUBLIST _SubList;
static uint8_t _crcBuf[COMM_SUB_LIST_MAX][2];

void comm_sub_reset(void){
    bzero((void*)&_rcv,sizeof(_rcv));
}
void comm_sub_add_list(uint8_t* pBuf){
    if( !ISEMPTY(_SubList.sof,_SubList.sof) ){
        memcpy(&_SubList.MBus[_SubList.sof],(MBUS_WRITE_SND*)pBuf,sizeof(MBUS_WRITE_SND));
        ADD_FLUSH(&_SubList.eof,COMM_SUB_LIST_MAX);
    }else{
        printf("comm sub list is full\n");
    }
}
void comm_sub_start_rcv(int ms){
    _time_out = getSystemTimeMilli() + ms;
}
bool comm_sub_checkPacket(uint8_t* pCrc16){
    if( _rcv.length < 3 ){
        return false;
    }
    uint16_t crc16 =(pCrc16[0]=_rcv.chars[_rcv.length-2])*256 + (pCrc16[1]=_rcv.chars[_rcv.length-1]);
    uint16_t crc16_ = CRC16((uint8_t*)_rcv.chars,_rcv.length-2); 
    if( crc16 != crc16_ ){
        return false;
    }
    return true;
}
bool comm_sub_checkCrcBuf(uint8_t* pCrc16){
    uint8_t index = _rcv.chars[0]-1;
    if( memcmp(_crcBuf[index],pCrc16,2) == 0 ){
        return false;    
    }
    memcpy(_crcBuf[index],pCrc16,2);
    return true;
}
uint8_t comm_sub_check(void){
    uint8_t crc16[2];
    if( !comm_sub_checkPacket(crc16) ){
        return CHECK_PACKET_ERROR;         
    }
    if( !comm_sub_checkCrcBuf(crc16)){
        return CHECK_CRCBUF_EQUAL;
    }
    return CHECK_PASS;
}
void comm_sub_sendPacket(uint8_t* pBuf,uint8_t len){
    uart_sendbytes(_fd_uart,pBuf,len);    
}
void comm_sub_exeMbus(void){
    comm_sub_reset();
    comm_sub_start_rcv(MBUS_RCV_MS);

    MBUS_WRITE_SND mbus;
    if( !ISEMPTY(_SubList.sof,_SubList.eof) ){
        memcpy(&mbus,&_SubList.MBus[_SubList.sof],sizeof(MBUS_WRITE_SND));
        comm_sub_sendPacket((uint8_t*)&mbus,sizeof(MBUS_WRITE_SND));
        DELETE_FLUSH(&_SubList.sof,COMM_SUB_LIST_MAX);
    }
}
void comm_sub_read_tick(void){
    if( getSystemTimeMilli() < _time_out ){
        uint8_t ch = uart_readbyte(_fd_uart);
        _rcv.chars[_rcv.length++] = ch;
        printf("comm sub tick ch=%d\n",ch);
    }
}

void comm_sub_loop(void* arg){
    comm_sub_read_tick();
}

void comm_sub_init(void){
    bzero((void*)&_SubList,sizeof(_SubList));
    comm_sub_reset();
    if( (_fd_uart = uart_open_port(COMM_SUB_PORT)) < 0 ){
        perror("Error:open serial port");
        exit(-1);
    }
    if( uart_set_opt(_fd_uart,9600,8,'N',1) < 0 ){
        perror("Error:set opt");
        exit(-1);
    }
}

void *comm_sub_thread(void* arg){
    pthread_detach(pthread_self());
    comm_sub_init();
    while(1){
        comm_sub_loop(arg);
    }
}

void comm_sub_start_th(void){
    if(  pthread_create(&_comm_sub_handle[0],NULL,comm_sub_thread,NULL) != 0 ){
        perror("pthread_create");
        exit(-1);
    }
}
