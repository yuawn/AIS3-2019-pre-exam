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

char usr[0x80] , buf[0x100];

void _register( char *pwd ){
    memset( usr , 0 , 0x80 );
    memset( pwd , 0 , 0x80 );
    printf( "Account: " );
    read( 0 , usr , 0x7f );
    printf( "Password: " );
    read( 0 , pwd , 0x7f );
    puts( "Success!" );
}


struct Box{
    int8_t size;
    char buf[0x100];
};

void box(){

    struct Box boxs[8];

    boxs[7].buf = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";


}

int main(){

    init();

    char pwd[0x80];

    while(1){
        menu();
        int n = read_int();

        switch( n ){
            case 1:
                _register( pwd );
                break;
            case 2:
                printf( "Login account: " );
                read( 0 , buf , 0x90 );
                if( strncmp( buf , usr , strlen( buf ) ) ){
                    puts( "No such user!" );
                    break;
                }
                printf( "Password : " );
                read( 0 , buf , 0x90 );
                if( strncmp( buf , pwd , strlen( buf ) ) ){
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