#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

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
    puts( "--------------------" );
    puts( "1. Say something." );
    puts( "2. Print." );
    puts( "3. Burn it up." );
    puts( "4. Bye." );
    puts( ">" );
}

char *ptr;

int main(){
    init();

    while( 1 ){
        menu();
        int n = read_int();
        switch(n){

            case 1:
                ptr = malloc( 0x18 );
                read( 0 , ptr , 0x18 );
                break;
            case 2:
                if( !ptr ){
                    puts( "Say something first!" );
                    break;
                }
                puts( ptr );
                break;
            case 3:
                free( ptr );
                break;
            case 4:
                puts( "Bye~" );
                _exit(0);
                break;
            default:
                break;
        }

    }

    return 0;
}