#include<stdio.h>
#include<stdlib.h>


void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

void main(){

    init();

    int n;
    char buf[0x10];

    read( 0 , buf , 0x10 + 8 + 1 );

    

}