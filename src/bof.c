#include<stdio.h>
#include<stdlib.h>


void welcome_to_ais3_2019(){
    system("sh");
}


void init(){
    setvbuf(stdout,0,2,0);
    setvbuf(stdin,0,2,0);
    setvbuf(stderr,0,2,0);
}

int main(){

    init();

    puts( "👻   They said there need some easy challenges, Okay here is your bof, but you should notice something in ubuntu 18.04." );

    char buf[0x30];

    gets( buf );

    return 0;
}