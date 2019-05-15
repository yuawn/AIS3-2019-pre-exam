#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#define MAX 10

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
    puts( "1. add a heap." );
    puts( "2. show a heap." );
    puts( "3. delete a heap." );
    puts( "4. Bye." );
    puts( ">" );
}

char *heap[MAX];

void add(){
    unsigned int size = 0 ;
    for( int i = 0 ; i < MAX ; ++i ){
        if( !heap[i] ){
            printf( "Size:" );
            size = read_int();
            heap[i] = (char*)malloc( size );
            printf( "Data:" );
            read( 0 , heap[i] , size );
            puts( "Done!" );
            return;
        }
    }
    puts( "Full!" );
}

void show(){
    unsigned int idx;
	printf("Index :");
	idx = (unsigned int)read_int();
	if(idx < MAX){
        if( !heap[idx] ){
            puts( "No such heap." );
            return;
        }
        puts( heap[idx] );
    }
}

void delete(){
    unsigned int idx;
	printf("Index :");
	idx = (unsigned int)read_int();
	if(idx < MAX){
        if( !heap[idx] ){
            puts( "No such heap." );
            return;
        }
        free( heap[idx] );
        puts( "Done!" );
    }
}

int main(){
    init();

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
                puts( "Bye~" );
                _exit(0);
                break;
            default:
                break;
        }

    }

    return 0;
}