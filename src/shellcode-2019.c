#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <seccomp.h>
#include <sys/mman.h>


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


int main(){

    init();


    char sc[0x100];

    puts( "Talk is cheap. Show me the shellcode." );

    read( 0 , sc , 0x100 );

    void (*ptr)() = (void(*)())mmap( NULL , 0x1000 , PROT_READ | PROT_WRITE | PROT_EXEC , MAP_PRIVATE | MAP_ANONYMOUS , -1 , 0 );

    memcpy( ptr , sc , 0x100 );

    enable_seccomp();

    ptr();

    return 0;
}