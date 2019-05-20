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

void box_menu(){
    puts( "1. new box" );
    puts( "2. update box" );
    puts( "3. view all box" );
    puts( "4. delete box" );
    puts( "5. Logout" );
    puts( "Your choice >" );
}

struct Box{
    int8_t size;
    char buf[0xe8];
};

void box(){
    puts( "Login successfully." );

    int i;
    struct Box boxs[8];

    memset( boxs , 0 , sizeof( boxs ) );

    while(1){
        box_menu();
        unsigned int idx;
        int n = read_int();

        switch( n ){
            case 1:
                for( i = 0 ; i < 8 ; ++i ){
                    if( !boxs[i].size ) break;
                }
                if( i >= 8 ){
                    puts( "No more box!" );
                    break;
                }
                printf( "Put something into the box > " );
                scanf( "%232s" , boxs[i].buf );
                if( strlen( boxs[i].buf ) < 2 ) boxs[i].size = 2;
                else boxs[i].size = strlen( boxs[i].buf );
                break;
            case 2:
                puts( "Which box?" );
                idx = read_int();
                if( idx >= 8 ){
                    puts( "Nop!" );
                    _exit(-1);
                }
                if( !strlen( boxs[idx].buf ) ){
                    puts( "No such box!" );
                    break;
                }
                printf( "New things > " );
                int ret = read( 0 , buf , (unsigned char)( boxs[idx].size - 1 ) );
                memcpy( boxs[idx].buf , buf , ret );
                puts( "Done!" );
                break;
            case 3:
                for( i = 0 ; i < 8 ; ++i ){
                    if( boxs[i].size ){
                        printf( "[Box %d] %s\n" , i , boxs[i].buf );
                    }
                }
                break;
            case 4:
                puts( "Which box?" );
                idx = read_int();
                if( idx >= 8 ){
                    puts( "Nop!" );
                    _exit(-1);
                }
                boxs[idx].size = 0;
                memset( boxs[idx].buf , 0 , 0xe8 );
                break;
            case 5:
                memset( boxs , 0 , sizeof( boxs ) );
                return;
            default:
                puts( "🤔🤔🤔🤔🤔🤔" );
                break;
        }
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
                if( memcmp( buf , usr , len ) ){
                    puts( "No such user!" );
                    break;
                }
                printf( "Password : " );
                len = read( 0 , buf , 0x98 );
                buf[len] = '\0';
                if( memcmp( buf , pwd , len ) ){
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