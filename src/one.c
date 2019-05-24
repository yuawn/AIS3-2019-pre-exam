#include<stdio.h>
#include<stdlib.h>


void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

char s[0x10];

void main(){

    init();

    int n = 0;
    char buf[0x18];

    read( 0 , buf , 0x10 + 8 + 1 );

    scanf( "%d" , &n );

    read( 0 , s , n );
    

}