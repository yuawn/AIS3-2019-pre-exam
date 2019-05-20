#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>


void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

int rand(){
    int ret;
    int fd = open( "/dev/urandom" , O_RDONLY ); 
    if( fd < 0 ){ 
        printf("error\n"); 
        _exit(1); 
    }
    read( fd , &ret , 4 );
    close( fd );

    return ret % 49 + 1;
}

void welcome(){
    puts( "+-----------------------------+" );
    puts( "+           LOTTERY           +" );
    puts( "+-----------------------------+" );
}

char buf[0x30];

int main(){

    init();

    long num[6] , a[6];

    for( int i = 0 ; i < 6 ; ++i ){
        num[i] = rand();
    }


    return 0;
}