#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int fd = 2;
char buf[0x30];

void init(){
    setvbuf(stdin,0,2,0);
    setvbuf(stdout,0,2,0);
    setvbuf(stderr,0,2,0);
}

void my_printf(){
    dprintf( fd , buf );
    return;
}

void black_hole(){
    while( read( 0 , buf , 0x30 ) > 0 ){
        if( !strncmp( buf , "exit" , 4 ) ){
            return;
        }
        my_printf();
    }
    return;
}

int main(){
    init();

    black_hole();

    return 0;
}