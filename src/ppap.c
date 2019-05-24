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
    char buf[0x10];

    puts( "Stack overflow." );
    read( 0 , buf , 0x20 + 8 + 1 );

    puts( "bss overflow." );
    printf( "How many bytes do you want to overflow? " );
    scanf( "%d" , &n );

    if( n > 0x100000 ){
        puts( "Wow you are so greedy, calm down bro, let's sing a song :P" );

        sleep( 0.5 );
        puts( "3" );
        sleep( 0.5 );
        puts( "2" );
        sleep( 0.5 );
        puts( "1" );
        sleep( 0.5 );

        puts( "I have a stack overlfow." );
        sleep( 1 );
        puts( "I have a bss overlfow." );
        sleep( 1.5 );
        puts( "Uh!" );
        sleep( 1 );
        printf( "S " );
        sleep( 0.5 );
        printf( "H " );
        sleep( 0.5 );
        puts( "Shell!" );
        sleep( 1 );
    }

    printf( "OK. Here is your bss overflow :) > " );
    read( 0 , s , n );
}