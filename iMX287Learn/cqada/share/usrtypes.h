#ifndef _USRTYPES_H_
#define _USRTYPES_H_
#include "stdint.h"
#include "stdbool.h"

typedef unsigned long long int gint64_t;

#define CHARS_MAXSIZE 128
typedef struct _ASCCHARS{
    uint8_t length;
    char  chars[CHARS_MAXSIZE];
}ASCCHARS;

typedef  ASCCHARS COMMRCV;

static inline bool ISEMPTY(uint8_t sof,uint8_t eof){
    return (sof == eof);
}
static inline bool ISFULL(uint8_t sof,uint8_t eof){
    return ( (eof+1) == sof );
}
static inline void DELETE_FLUSH(uint8_t* pSof,uint8_t max){
    *pSof = (*pSof+1) % max;
}
static inline void ADD_FLUSH(uint8_t* pEof,uint8_t max){
    *pEof = (*pEof+1) % max;
}


#endif

