#include "stdio.h"
#include "unistd.h"
#include "stdint.h"

#include "../uart/comm_sub.h"
#include "../share/usrtime.h"
#include "../share/usrtypes.h"

gint64_t _timeout = 0;

void main_init(void){
    comm_sub_start_th();
}

void main_loop(void){
}

int main(int argc,char** argv){
    main_init();    

    while(1){
        main_loop();
    }
    return 0;
}
