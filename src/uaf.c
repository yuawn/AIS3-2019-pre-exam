#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#define MAX 20

void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

struct Msg{
    char gap[0x90];
    void (*hello)();
    void (*bye)();
};

void hello_msg(){
    puts( "Hello!" );
}

void bye_msg(){
    puts( "Good bye!" );
}

void printf_flag(){
    system("cat /home/uaf/flag");
}

int read_int(){
    char buf[0x10];
    __read_chk( 0 , buf , 0xf , 0x10 );
    return atoi( buf );
}

void menu(){
    puts( "--------------------" );
    puts( "1. add messege." );
    puts( "2. show all meseege." );
    puts( "3. delete messege." );
    puts( "4. Bye." );
    puts( ">" );
}

char *heap[MAX];

void add(){
    unsigned int size = 0 ;
    for( int i = 0 ; i < MAX ; ++i ){
        if( !heap[i] ){
            printf( "Messege size:" );
            size = read_int();
            heap[i] = (char*)malloc( size );
            printf( "Messege:" );
            int r = read( 0 , heap[i] , size - 1 );
            if( heap[i][r - 1] == '\n' ){
                heap[r - 1] = '\0';
            }
            else{
                heap[r] = '\0';
            }
            puts( "Done!" );
            return;
        }
    }
    puts( "Full!" );
}

void show(){
    for( int i = 0 ; i < MAX ; ++i ){
        if( heap[i] ){
            printf( "Msg %d: %s\n" , i , heap[i] );
        }
    }
}

void delete(){
    unsigned int idx;
	printf("Index :");
	idx = (unsigned int)read_int();
	if(idx < MAX){
        if( !heap[idx] ){
            puts( "No such messege." );
            return;
        }
        free( heap[idx] );
        heap[idx] = 0;
        puts( "Done!" );
    }
}

int main(){
    init();

    struct Msg *msgbox = (struct Msg*)malloc( sizeof( struct Msg ) );

    msgbox->hello = hello_msg;
    msgbox->bye = bye_msg;

    msgbox->hello();

    while( 1 ){
        menu();
        int n = read_int();
        switch(n){

            case 1:
                add();
                break;
            case 2:
                show();
                break;
            case 3:
                delete();
                break;
            case 4:
                msgbox->bye();
                free(msgbox);
                break;
            default:
                puts( "Umm" );
                break;
        }

    }

    return 0;
}