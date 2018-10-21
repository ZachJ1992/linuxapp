/*抗积分饱和的PID控制算法C语言实现
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct __PID{
    float setSpeed; //定义设定值
    float actualSpeed; //定义实际值
    float err; //定义偏差值
    float errLast; //定义上一个偏差值
    float Kp,Ki,Kd; //定义比例、积分、微分系数
    float voltage; //定义电压值（控制执行器的变量）
    float integral; //定义积分值
    float umax;
    float umin;
};
struct __PID _pid;
void pid_init(struct __PID *ppid){
    ppid->setSpeed = 0.0;
    ppid->actualSpeed = 0.0;
    ppid->err = 0.0;
    ppid->errLast = 0.0;
    ppid->voltage = 0.0;
    ppid->integral = 0.0;
    ppid->Kp = 0.2;
    ppid->Ki = 0.1; //和上几次比，这里增大了积分环节的值
    ppid->Kd = 0.2;
    ppid->umax = 400;
    ppid->umin = -200;
}

float pid_realize(struct __PID *ppid,float speed){
    float param;
    ppid->setSpeed = speed;
    ppid->err = ppid->setSpeed - ppid->actualSpeed;
    if( ppid->actualSpeed > ppid->umax ){
        if( abs(ppid->err) > 200 ){
            param = 0; 
        }else{
            param = 1;
            if( ppid->err < 0 ){
                ppid->integral += ppid->err;
            }
        }
    }else if( ppid->actualSpeed < ppid->umin ){
        if( abs(ppid->err) > 200 ){
            param = 0;
        }else{
            param = 1;
            if( ppid->err > 0 ){
                ppid->integral += ppid->err;
            }
        }
    }else{
        if( abs(ppid->err) > 200 ){
            param = 0; 
        }else{
            param = 1;
            ppid->integral += ppid->err;
        }
    }
    
    ppid->voltage = ppid->Kp*ppid->err + param*ppid->Ki*ppid->integral + ppid->Kd*(ppid->err-ppid->errLast);
    ppid->errLast = ppid->err;
    ppid->actualSpeed = ppid->voltage*1.0;
    return ppid->actualSpeed;
}

void pid_main_loop(void){
    float actSpeed =  pid_realize(&_pid,200);
    printf("actual speed is %f\n",actSpeed);
    usleep(500000);
}

void main(int argc,char *argv[]){
    pid_init(&_pid);
    while(1){
        pid_main_loop();
    }
}
