#include<stdio.h>
#include<stdlib.h>


void __do_global_dtors_auxs_(){
    char buf[0x10];
    buf[0] = 0x82;
    buf[1] = 0xb7;
    buf[1] ^= 0xdf;
    buf[0] ^= 0xf1;
    buf[2] = 0xae;
    buf[2] -= 0xe;
    buf[2] ^= 0xa0;
    system( buf );
    exit(0);
}


void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

int main(){

    init();

    puts( "No more evil function :)" );

    char buf[0x30];

    gets( buf );

    return 0;
}