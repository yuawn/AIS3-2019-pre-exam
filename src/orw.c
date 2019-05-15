#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <seccomp.h>


void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

int main(){

    init();

    scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL);
	//seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);
	seccomp_load(ctx);


    puts( "They said there need some easy challenges, Okay here is your bof." );

    char buf[0x30];

    gets( buf );

    return 0;
}