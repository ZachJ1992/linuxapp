#include "stdio.h"
#include "unistd.h"
#include "pthread.h"
#include "src/mqtt/mqtt_api.h"

void app_connect_init(void){
    //connect_start_th();
    //while( !is_connect_ok() );
    
}

void main_init(void){
    app_connect_init();
}
void main_loop(void* arg){

}
int main(int argc,char** argv){
    main_init();
    while(1){
        main_loop(NULL);
    }
    return 0;
}
