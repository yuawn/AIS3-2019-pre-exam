#include<stdio.h>
#include<stdlib.h>

void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);

    srand( time( NULL ) );
}

int read_int(){
    char buf[0x10];
    __read_chk( 0 , buf , 0xf , 0x10 );
    return atoi( buf );
}

char saved_canary[0x10];

void gen_canary(){
    for( int i = 0 ; i < 0x10 ; ++i ){
        saved_canary[i] = rand() & 0xff;
    }
    puts( "[+]Generate secure random canary...done" );
}

int main(){
    init();

    char buf[0x100] , canary[0x10];

    memset( buf , 0 , 0x100 );

    int size;

    puts( "I have implemented the secure canary and OOB checking, I believe it's a really safe program :D" )
    puts( "Length of your input>" );

    gen_canary();
    memcpy( canary , saved_canary , 0x10 );

    size = read_int();

    if( size < 0 ){
        puts( "Hacks detected! abs() your len :)" );
        size = abs( size );
    }

    if( size > 0xff ){
        puts( "Too long!" );
        size = 0xff;
    }

    puts( "Your input>" );

    read( 0 , buf , size );

    puts( "[+]Check for secure canary." );

    if( strncmp( canary , saved_canary , 0x10 ) ){
        puts( "[ERROR] Overflow detected!" );
        _exit(-1);
    }
    
    puts( "Everthing works fine :)" );

    return 0;
}