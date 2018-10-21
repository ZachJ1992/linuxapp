//判断车流量的方案：假设车辆均正常通过
//入方向，X首先被触发，那么在可预见的时间内，Y会被触发，并且Y点时间坐标必定大于X点时间坐标；
//出方向，Y首先被触发，那么在可预见的时间内，X会被触发，并且X点时间坐标必定大于Y点时间坐标。
//vim /usr/local/arm/arm-2009q3/arm-none-linux-gnueabi/libc/usr/include/sys/time.h
#include "includes/includes.h"
#include "timer/gettime.h"
#include "driver/config.h"


void app_main_init(void){
    fns_detec_init();
    fns_udp_init();
}
void app_main_load(void){
}
void app_main_loop(void){
    app_detec_loop();
    app_udp_loop();
}

int main(int argc,char **argv){
    app_main_init();
    app_main_load();
    while(1){
        app_main_loop();    
    }
}
