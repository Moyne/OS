#include <signal.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DIM 50

int main(int argc,char** argv){
    char buf[DIM];
    read(0,&buf[0],DIM);
    execlp("echo","echo",buf,(char*)0);
}
