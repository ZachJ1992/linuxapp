/*积分分离的PID 控制算法C语言实现
  以增量型PID算法C语言实现为例
  在PID控制中，引入积分环节的目的，主要是为了消除静差，提高控制精度。
但在启动、结束或大幅度增减设定时，短时间内系统输出有很大的偏差，会造成PID
运算的积累，导致控制量超过执行机构可能允许的最大动作范围对应极限控制量，
从而引起较大的超调，甚至是震荡，这是绝对不允许的。
  为了克服这一问题，引入了积分分离的概念，基本思路是当被控量与设定值偏差
较大时，取消积分作用；当被控量接近给定值时，引入积分控制，以消除静差，
提高精度。
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct __PID{
    float setSpeed; //定义设定值
    float actualSpeed; //定义实际值
    float err; //定义偏差值
    float errNext; //定义上一个偏差值
    float errLast; //定义最上前的偏差值
    float Kp,Ki,Kd; //定义比例、积分、微分系数
};

struct __PID _pid;
void pid_init(struct __PID *ppid){
    ppid->setSpeed = 0.0;
    ppid->actualSpeed = 0.0;
    ppid->err = 0.0;
    ppid->errNext = 0.0;
    ppid->errLast = 0.0;
    ppid->Kp = 0.2;
    ppid->Ki = 0.04;
    ppid->Kd = 0.2;
}
float pid_realize(struct __PID *ppid,float speed){
    float param; //用于控制积分项
    ppid->setSpeed = speed;
    ppid->err = ppid->setSpeed - ppid->actualSpeed;
    if( abs(ppid->err) > 200 ){
        param = 0;
    }else{
        param = 1;
    }
    //速度增量
    float incrementSpeed = ppid->Kp*(ppid->err-ppid->errNext) + param*ppid->Ki*ppid->err + ppid->Kd*(ppid->err-2*ppid->errNext+ppid->errLast);
    ppid->actualSpeed += incrementSpeed;
    ppid->errLast = ppid->errNext;
    ppid->errNext = ppid->err;
    return ppid->actualSpeed;  
}
void pid_main_loop(void){
    float actSpeed = pid_realize(&_pid,200.0);
    printf("actual speed is %f\n",actSpeed);
    usleep(500000);
}
void main(int argc,char *argv[]){
    pid_init(&_pid);
    while(1){
        pid_main_loop();
    }
}

