#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

//typedef signed char       int8_t;

void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

int read_int(){
    char buf[0x10];
    __read_chk( 0 , buf , 0xf , 0x10 );
    return atoi( buf );
}



int rand(){
    unsigned int ret = 0;
    int fd = open( "/dev/urandom" , O_RDONLY ); 
    if( fd < 0 ){ 
        printf("error\n"); 
        _exit(1); 
    }
    read( fd , &ret , 4 );
    close( fd );

    return ret % 49 + 1;
}

void menu(){
    puts( "+-----------------------------+" );
    puts( "|             Box             |" );
    puts( "+-----------------------------+" );
    puts( "1. Register" );
    puts( "2. Login" );
    puts( "3. Exit" );
    puts( "Your choice >" );
}

char usr[0x88] , buf[0x100];

void _register( char *pwd ){
    memset( usr , 0 , 0x88 );
    memset( pwd , 0 , 0x88 );
    printf( "Account: " );
    read( 0 , usr , 0x80 );
    printf( "Password: " );
    read( 0 , pwd , 0x80 );
    puts( "Success!" );
}


struct Box{
    int8_t size;
    char buf[0x100 - 0x18];
};

void box(){

    struct Box boxs[8];

    boxs[6].size = 0x11;
    boxs[7].size = 0x66;
    for( int i = 0 ; i < 0x100 - 0x18 ; ++i ){
        boxs[6].buf[i] = 'a';
        boxs[7].buf[i] = 'b';
    }


}

int main(){

    init();

    char pwd[0x80];

    while(1){
        menu();
        int len;
        int n = read_int();

        switch( n ){
            case 1:
                _register( pwd );
                break;
            case 2:
                printf( "Login account: " );
                len = read( 0 , buf , 0x98 );
                buf[len] = '\0';
                if( strncmp( buf , usr , len ) ){
                    puts( "No such user!" );
                    break;
                }
                printf( "Password : " );
                len = read( 0 , buf , 0x98 );
                buf[len] = '\0';
                if( strncmp( buf , pwd , len ) ){
                    puts( "Wrong password!" );
                    break;
                }
                box();
                break;
            case 3:
                puts( "Bye." );
                _exit(0);
                break;
            default:
                puts( "🤔🤔🤔🤔🤔" );
                break;
        }
    }


    return 0;
}