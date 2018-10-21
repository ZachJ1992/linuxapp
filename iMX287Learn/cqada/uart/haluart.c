#include "stdio.h"
#include "unistd.h"
#include "strings.h"
#include "termios.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdint.h"
#include "haluart.h"

int uart_open_port(int port){
    int fd;
    char *dev[] = {"/dev/ttySP0","/dev/ttySP1","/dev/ttySP2"};
    if( port == 1 ){
        fd = open(dev[0],O_RDWR|O_NOCTTY|O_NDELAY);
        if( fd == -1 ){
            perror("Can't Open Serial Port");
            return -1;
        }
    }else if( port == 2 ){ 
        fd = open(dev[1],O_RDWR|O_NOCTTY|O_NDELAY);
        if( fd == -1 ){
            perror("Can't Open Serial Port");
            return -1;
        }
    }else if( port == 3 ){
        fd = open(dev[2],O_RDWR|O_NOCTTY|O_NDELAY);
        if( fd == -1 ){
            perror("Can't Open Serial Port");
            return -1;
        }
    }else{
        perror("Port number");
        return -1;
    }
    
    //恢复串口为阻塞状态
    int ret = fcntl(fd,F_SETFL,0);
    if( ret < 0 ){
        printf("fcntl failed!\n");
    }else{
        printf("fcntl=%d\n",ret);
    }

    //测试是否为终端设备
    if( isatty(STDIN_FILENO) == 0 ){
        printf("standard input is not a terminal device\n");
    }else {
        printf("isatty success!\n");
    }

    printf("fd-open=%d\n",fd);
    return fd;
}

int uart_set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop){
    struct termios newtio,oldtio;
    
    //保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息
    if( tcgetattr(fd,&oldtio) != 0 ){
       perror("SetupSerial");
        return -1;
    }
    bzero(&newtio,sizeof(newtio));
    //忽略调制解调器状态行，启动接收
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
    
    //设置数据位大小
    switch( nBits ){
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    //设置奇偶校验位
    switch( nEvent ){
        case 'O'://奇数
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E'://偶数
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N'://无奇偶校验位
            newtio.c_cflag &= ~PARENB;
            break;
    }

    //设置波特率
    switch( nSpeed ){
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        case 460800:
            cfsetispeed(&newtio, B460800);
            cfsetospeed(&newtio, B460800);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }

    //设置停止位
    if( nStop == 1 ){
        newtio.c_cflag &= ~CSTOPB;
    }else if( nStop == 2 ){
        newtio.c_cflag |= CSTOPB;
    }

    //设置等待时间和最小接收字符
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;    

    //处理未接收字符
    tcflush(fd,TCIFLUSH);
    
    //激活新配置
    if( (tcsetattr(fd,TCSANOW,&newtio)) != 0 ){
        perror("com set");
        return -1;
    }

    printf("com set done!\n");
    return 0;
}


void uart_sendbyte(int fd,const uint8_t byte){
    if( write(fd,&byte,1) < 0 ){
        printf("write data to serial failed!\n");
    }
}

void uart_sendbytes(int fd,uint8_t* pBuf,size_t len){
    if( write(fd,pBuf,len) < 0 ){
        printf("write data to serial failed!\n");
    }
} 
uint8_t uart_readbyte(int fd){
    uint8_t buf[1];
    int byte = read(fd,buf,1);
    if( byte == 1 ){
      return buf[0];
    }
    return byte;
}

