#include "../includes/includes.h"
#include "gettime.h"

struct timeval _tv0,_tv1;
struct timezone _tz;

struct timeval *get_timeval_now(struct timeval *ptv){
    gettimeofday(&_tv0,&_tz);
    if(ptv!=NULL){
        memcpy(ptv,&_tv0,sizeof(struct timeval));            
    }
    return &_tv0;
}

struct timeval *get_timeval_ms(long ms){
    timerclear(&_tv1);
    if( ms >= 1000 ){
        _tv1.tv_sec = ms/1000;
        _tv1.tv_usec = (ms%1000)*1000;
    }else{
        _tv1.tv_usec = ms*1000;
    }
    return &_tv1;
}
