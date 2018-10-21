#include "../includes/includes.h"
#include "../timer/gettime.h"
#include "../driver/config.h"
#include "detec.h"

int _fd;
struct _CARDET _cardet[CHANNEL_MAX];
static struct timeval _fnstick;
void fns_detec_init(void){
    system("bash /opt/load.sh");
    sleep(1);
    _fd = open(CARDET_DEV_PATH,O_RDWR);
    if( _fd < 0 ){
        perror("Open "CARDET_DEV_PATH"failed!\n");
        exit(1);
    }   
}
void fns_detec_load(void){
    
}

int fns_cardet_getflows(int ch){
    if( ch > CHANNEL_MAX ){
        printf("error : ch > CHANNEL_MAX\n");
        return -1;
    }
    return _cardet[ch].flows; 
}

void fns_cardet_doflows(struct _CARDET *pCardet){
    if( pCardet->state == CARDET_STATE_SCAN ){ //检测通道点
        if( timerisset(&pCardet->tmsign.xp.fallingedge) || timerisset(&pCardet->tmsign.yp.fallingedge) ){
            //通道点被触发（可能是一个，也可能是两个）
            pCardet->state = CARDET_STATE_TRIGGER;
        }           
    }else if( pCardet->state == CARDET_STATE_TRIGGER ){
        pCardet->state = CARDET_STATE_WAIT;
        timeradd(get_timeval_now(NULL),get_timeval_ms(1000),&pCardet->timeout);
    }else if( pCardet->state == CARDET_STATE_WAIT ){
        if( timerisset(&pCardet->tmsign.xp.fallingedge) && timerisset(&pCardet->tmsign.yp.fallingedge) ){
            //两个通道均被触a发
            pCardet->state = CARDET_STATE_PASSED;
        }
        if( timercmp(get_timeval_now(NULL),&pCardet->timeout,>) ){
            //可预见的时间内车辆没有通过
            pCardet->state = CARDET_STATE_END;
        }    
    }else if( pCardet->state == CARDET_STATE_PASSED ){
        //printf("xp.fallingedge.tv_sec=%ld,xp.fallingedge.tv_usec=%ld\n",pCardet->tmsign.xp.fallingedge.tv_sec,pCardet->tmsign.xp.fallingedge.tv_usec);
        //printf("yp.fallingedge.tv_sec=%ld,yp.fallingedge.tv_usec=%ld\n",pCardet->tmsign.yp.fallingedge.tv_sec,pCardet->tmsign.yp.fallingedge.tv_usec);
        if( timercmp(&pCardet->tmsign.xp.fallingedge,&pCardet->tmsign.yp.fallingedge,>) ){
            pCardet->flows--;
        }else if( timercmp(&pCardet->tmsign.xp.fallingedge,&pCardet->tmsign.yp.fallingedge,<) ){
            pCardet->flows++;
        }
        printf("car flow(s) : %d\n",pCardet->flows);
        pCardet->state = CARDET_STATE_END;
    }else if( pCardet->state == CARDET_STATE_END ){
        timerclear(&pCardet->tmsign.xp.fallingedge); //清除x点下降沿时标
        timerclear(&pCardet->tmsign.yp.fallingedge); //清除y点下降沿时标
        
        pCardet->tcls = 1;
        pCardet->state = CARDET_STATE_SCAN;    
    }
}
void fns_detec_loop(void){
    int i,ret;
    struct timeval tv;
    
    //首先，应读取驱动层时间坐标
    ret = read(_fd,(char*)&_cardet[0].tmsign,CHANNEL_MAX*sizeof(struct _TIMESIGN));
    if( ret < 0 ){
        perror("read() err");
        exit(1);
    }

    for(i=0;i<CHANNEL_MAX;i++){
        fns_cardet_doflows(&_cardet[i]);
        if( _cardet[i].tcls == 1 ){
            get_timeval_now(&tv);
            //printf("write(_fd,(char*)&_cardet[0].tmsign...\n tv.tv_sec=%ld,tv.tv_usec=%ld\n",tv.tv_sec,tv.tv_usec);
            //将数据更新到驱动层，主要是清楚时标
            ret = write(_fd,(char*)&_cardet[0].tmsign,CHANNEL_MAX*sizeof(struct _TIMESIGN));
            fsync(_fd);
            if( ret < 0 ){
                perror("write() err");
                exit(1);
            }
            _cardet[i].tcls = 0;
        }
    }
    usleep(1000);
}

void app_detec_loop(void){
    if( timercmp(get_timeval_now(NULL),&_fnstick,>) ){
        fns_detec_loop();
        timeradd(get_timeval_now(NULL),get_timeval_ms(1),&_fnstick);
    }
}

