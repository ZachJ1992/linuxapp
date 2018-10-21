#include "stdio.h"
#include "stdint.h"
#include "sys/time.h"
#include "usrtime.h"

//毫秒
gint64_t getSystemTimeMilli(void){
    struct timeval tve;
    gettimeofday(&tve,NULL);
    gint64_t sec = tve.tv_sec;
    gint64_t microseconds = tve.tv_usec;
    gint64_t ret = sec*(1000ll)+microseconds/(1000ll);
    return ret;
}
//纳秒
gint64_t getSystemTimeNano(void){
    struct timeval tve;
    gettimeofday(&tve,NULL);
    gint64_t sec = tve.tv_sec;
    gint64_t microseconds = tve.tv_usec;
    gint64_t ret = sec*(1000ll*1000ll*1000ll)+microseconds*(1000ll);   
    return ret;
}
//毫秒
gint64_t getSystemTimeMicro(void){
    struct timeval tve;
    gettimeofday(&tve,NULL);
    gint64_t sec = tve.tv_sec;
    gint64_t microseconds = tve.tv_usec;
    gint64_t ret = sec*(1000ll*1000ll)+microseconds;   
    return ret;
}

