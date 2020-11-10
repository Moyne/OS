#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DIM 5

void son(long pos,char* fileName){
    int fp=open(fileName,O_RDWR | O_SYNC);
    lseek(fp,pos*sizeof(int),SEEK_SET);
    int x,y,rd;
    rd=read(fp,&x,sizeof(int));
    if(rd==-1){close(fp);exit(1);}
    else printf("I was in pos=%ld and i read x=%d\n",pos,x);
    rd=read(fp,&y,sizeof(int));
    if(rd==-1){close(fp);exit(1);}
    else printf("I was in pos=%ld and i read y=%d\n",pos,y);
    if(x>y){
        lseek(fp,pos,SEEK_SET);
        write(fp,&y,sizeof(int));
        write(fp,&x,sizeof(int));
    }
    close(fp);
    exit(1);
}

int main(int argc,char** argv){
    if(argc!=3) exit(1);
    pid_t pid;
    int n=atoi(argv[2]);
    for(long i=0;i<n-1;i++){
        for(long j=0;j<n-1-i;j++){
            pid=fork();
            if(!pid)    son(j,argv[1]);
            //sleep(1);
        }
        for(int j=0;j<n-1-i;j++) wait(NULL);
    }
    exit(1);
}
