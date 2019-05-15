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

    char buf[0x50];

    int fd = open( "/dev/urandom" , O_RDONLY ); 
    if( fd < 0 ){ 
        printf("error\n"); 
        _exit(1); 
    }
    read( fd , secret , 0x10 );
    close( fd );

    printf( "Input:" );
    read( 0 , buf , 0x20 );
    printf( buf );

    printf( "Input:" );
    read( 0 , buf , 0x20 );

    if( !strncmp( buf , secret , 0x10 ) ){
        system( "sh" );
    }
    else{
        puts( "Bye!" );
        _exit(0);
    }

    return 0;
}