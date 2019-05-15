#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

int main(){
    init();

    char buf[0x70];

    scanf( "%100s" , buf );
    printf( buf );
    exit(0);

    return 0;
}