/*积分分离的PID 控制算法C语言实现
  以位置型PID算法C语言实现
  在PID控制中，引入积分环节的目的，主要是为了消除静差，提高控制精度。
但在启动、结束或大幅度增减设定时，短时间内系统输出有很大的偏差，会造成PID
运算的积累，导致控制量超过执行机构可能允许的最大动作范围对应极限控制量，
从而引起较大的超调，甚至是震荡，这是绝对不允许的。
  为了克服这一问题，引入了积分分离的概念，基本思路是当被控量与设定值偏差
较大时，取消积分作用；当被控量接近给定值时，引入积分控制，以消除静差，
提高精度。

  注意：这里用基本的算法实现形式，没有考虑死去问题，没有设定上下限，
  只是对公式的一种直接实现，后面的介绍当中会逐渐的对此改进*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


struct __PID{
    float setSpeed; //定义设定值
    float actualSpeed; //定义实际值
    float err; //定义偏差值
    float errLast; //定义上一个偏差值
    float Kp,Ki,Kd; //定义比例、积分、微分系数
    float voltage; //定义电压值，控制执行器的变量
    float integral; //定义积分值
};

struct __PID _pid;
void pid_init(struct __PID *ppid){
    //统一初始化变量，尤其是Kp、Ki和Kd三个参数，调试过程中，
    //对要求的控制效果，可以通过调节这三个量直接进行调节。
    ppid->setSpeed = 0.0;
    ppid->actualSpeed = 0.0;
    ppid->err = 0.0;
    ppid->errLast = 0.0;
    ppid->Kp = 0.2;
    ppid->Ki = 0.15;
    ppid->Kd = 0.2;
    printf("pid init end.\n");
}

//该函数以一定周期T被调用
float pid_realize(struct __PID *ppid,float speed){
    float param;
    ppid->setSpeed = speed;
    ppid->err = ppid->setSpeed - ppid->actualSpeed;
    if( abs(ppid->err) > 200 ){
        param = 0;
    }else{
        param = 1;
        ppid->integral += ppid->err; //积分
    }
    ppid->voltage = ppid->Kp*ppid->err + param*ppid->Ki*ppid->integral + ppid->Kd*(ppid->err-ppid->errLast);
    ppid->errLast = ppid->err;
    ppid->actualSpeed = ppid->voltage*1.0;
    return ppid->actualSpeed; 
}
void app_main_loop(void){
    //每500ms输出一次
    float actSpeed = pid_realize(&_pid,200.0);
    printf("actual speed is %f\n",actSpeed);
    usleep(500000);
}
int main(int argc,char *argv[]){
    pid_init(&_pid);
    while(1){
        app_main_loop();   
    }
}
