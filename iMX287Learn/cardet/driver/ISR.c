#include "header.h"

struct timeval _t;
extern struct _TIMESIGN _timesign[CHANNEL_MAX];
 
static struct timeval *get_timeval_now(struct timeval *pt){
    do_gettimeofday(&_t);
    if(pt!=NULL){
        memcpy(pt,&_t,sizeof(struct timeval));            
    }
    return &_t;
}
//timeval_compare(get_timeval_now(),&_t);

irqreturn_t isr_ch0_xphandle(unsigned int irq,void *dev_id){
    get_timeval_now(&_timesign[0].xp.fallingedge);
    printk(KERN_INFO "isr_ch0_xphandle,sec=%ld,usec=%ld\n",_timesign[0].xp.fallingedge.tv_sec,_timesign[0].xp.fallingedge.tv_usec);
    return IRQ_HANDLED;
}
irqreturn_t isr_ch0_yphandle(unsigned int irq,void *dev_id){
    get_timeval_now(&_timesign[0].yp.fallingedge);
    printk(KERN_INFO "isr_ch0_yphandle,sec=%ld,usec=%ld\n",_timesign[0].yp.fallingedge.tv_sec,_timesign[0].yp.fallingedge.tv_usec);
    return IRQ_HANDLED;
}
