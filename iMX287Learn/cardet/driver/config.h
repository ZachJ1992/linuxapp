#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CH0_GPIO_No     68  //GPIO2_4
#define CH0_XP_GPIO_No  68  //GPIO2_4
#define CH0_YP_GPIO_No  69  //GPIO2_5
#define CH1_XP_GPIO_No  70  //GPIO2_6
#define CH1_YP_GPIO_No  71  //GPIO2_7


#define CHANNEL_MAX     1     


//应用程序中车检状态
#define CARDET_STATE_SCAN       0x00 //监测各个通道有无触发
#define CARDET_STATE_TRIGGER    0x01 //有触发
#define CARDET_STATE_WAIT       0x02 //等待车辆通过
#define CARDET_STATE_PASSED     0x03 //车辆已通过，即两个点被触发
#define CARDET_STATE_END        0x04 //结束


struct _XYPOINT{
    struct timeval risingedge;
    struct timeval fallingedge;
};

struct _TIMESIGN{
    struct _XYPOINT xp;
    struct _XYPOINT yp;
};

struct _CARDET{
    int state;
    int flows;
    unsigned char tcls;     //1表示有需要清除的时标
    struct timeval timeout;
    struct _TIMESIGN tmsign;
};


#endif


