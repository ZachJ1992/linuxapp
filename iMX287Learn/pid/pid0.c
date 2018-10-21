/*位置型PID算法C语言实现
  注意：这里用基本的算法实现形式，没有考虑死去问题，没有设定上下限，
  只是对公式的一种直接实现，后面的介绍当中会逐渐的对此改进*/
#include <stdio.h>
#include <unistd.h>

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
    ppid->setSpeed = speed;
    ppid->err = ppid->setSpeed - ppid->actualSpeed;
    ppid->integral += ppid->err; //积分
    ppid->voltage = ppid->Kp*ppid->err + ppid->Ki*ppid->integral + ppid->Kd*(ppid->err-ppid->errLast);
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
