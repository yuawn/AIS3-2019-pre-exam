#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>


void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

char flag[0x100];

void read_flag(){
    int fd = open( "/home/leaked-flag/flag" , O_RDONLY ); 
    if( fd < 0 ){ 
        printf("error\n"); 
        _exit(1); 
    }

    read( fd , flag , 0x30 );

    return;
}

void main(){

    init();

    read_flag();

    char key[0x10] , rnd[0x10];

    int fd = open( "/dev/urandom" , O_RDONLY ); 
    if( fd < 0 ){ 
        printf("error\n"); 
        _exit(1); 
    }

    read( fd , rnd , 0x10 );

    printf( "Your key to encrypt flag > " );

    read( 0 , key , 0x10 );

    printf( "[OK] Your key is %s\n" , key );

    for( int i = 0 ; i < 0x30; ++i ){
        flag[i] ^= key[ i % 0x10 ] + rnd[ i % 0x10 ];
    }

    printf( "Your flag: %s" , flag );

    return 0;
}