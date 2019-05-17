#include<stdio.h>
#include<stdlib.h>

void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

int read_long(){
    char buf[0x10];
    __read_chk( 0 , buf , 0xf , 0x10 );
    return atoi( buf );
}

int main(){
    init();

    char buf[0x100];

    long size = 0x80000000;

    printf( "%d %d\n" , size , abs( size ) )
    

    return 0;
}