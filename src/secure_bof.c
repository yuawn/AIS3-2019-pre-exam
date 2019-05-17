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

    int len = 0;
    char buf[0x110];

    memset( buf , 0 , 0x110 );

    gen_canary();
    memcpy( buf + 0x100 , saved_canary , 0x10 );
    //memcpy( canary , saved_canary , 0x10 );

    puts( "I have implemented the secure canary and OOB checking, I believe it's a really safe program :D" );
    puts( "Length of your input>" );

    scanf( "%d" , &len );
    //size = read_int();

    if( len <= 0 ){
        puts( "Hacks detected! abs() your len :)" );
        len = (int)abs( len );
    }

    if( len > 0xff ){
        puts( "Too long!" );
        len = 0xff;
    }

    puts( "Your input>" );

    read( 0 , buf , len );

    puts( "[+]Check for secure canary." );

    if( strncmp( buf + 0x100 , saved_canary , 0x10 ) ){
        puts( "[Canary] Overflow detected!" );
        _exit(-1);
    }
    
    puts( "Everthing works fine :)" );

    return 0;
}