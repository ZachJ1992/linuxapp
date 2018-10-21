/*增量型PID算法C语言实现
*/
#include <stdio.h>
#include <unistd.h>

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
    ppid->Ki = 0.015;
    ppid->Kd = 0.2;
}
float pid_realize(struct __PID *ppid,float speed){
    ppid->setSpeed = speed;
    ppid->err = ppid->setSpeed - ppid->actualSpeed;
    //速度增量
    float incrementSpeed = ppid->Kp*(ppid->err-ppid->errNext) + ppid->Ki*ppid->err + ppid->Kd*(ppid->err-2*ppid->errNext+ppid->errLast);
    ppid->actualSpeed += incrementSpeed; //积分计算
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

