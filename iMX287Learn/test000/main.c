#include "stdio.h"
#include "unistd.h"
#include "pthread.h"

pthread_t _thread_handle[1];

void threadLoop(void* arg){
    while(1){
        printf("thread \n");
        sleep(1);
    }
}

void *start_routine(void* arg){
    pthread_detach(pthread_self());
    threadLoop(arg);
}

void main_init(void){
    pthread_create(&_thread_handle[0],NULL,start_routine,NULL);
} 
int main(int argc,char** argv){
    main_init();    

    int i; 
    while(1){
        printf("hello %d\n",i++);
        sleep(1);
    }
    return 0;
}
