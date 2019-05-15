#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

char secret[0x10];

int main(){
    init();

    char buf[0x40];

    read( 0 , buf , 0x30 );

    printf( buf );

    exit(0);
}