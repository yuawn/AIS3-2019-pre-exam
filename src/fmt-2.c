#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

long magic = 0;

int main(){
    init();

    char buf[0x50];

    printf( "Input:" );
    read( 0 , buf , 0x4f );
    printf( buf );

    if( magic == 0xfaceb00c ){
        system( "sh" );
    }
    else{
        puts( "Bye!" );
        _exit(0);
    }

    return 0;
}