#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <seccomp.h>
#include <sys/mman.h>
#include<fcntl.h>


void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

void enable_seccomp(){
    scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL);
	seccomp_load(ctx);
}

char flag[0x100];

void read_flag(){
    int fd = open( "flag" , O_RDONLY ); 
    if( fd < 0 ){ 
        printf("error\n"); 
        _exit(1); 
    }
    read( fd , flag , 0x100 );
    close( fd );
}

int main(){

    init();

    read_flag();

    char sc[0x100];

    puts( "Talk is cheap. Show me the shellcode." );

    read( 0 , sc , 0x100 );

    void (*ptr)() = (void(*)())mmap( NULL , 0x1000 , PROT_READ | PROT_WRITE | PROT_EXEC , MAP_PRIVATE | MAP_ANONYMOUS , -1 , 0 );

    if( ptr < 0 ){
        puts( "mmap error" );
        _exit(0);
    }

    memcpy( ptr , sc , 0x100 );

    puts( "Runing..." );

    enable_seccomp();

    ptr();

    return 0;
}