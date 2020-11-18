#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DIM 50

void createString(char* buf,int rd){
    int i;
    for(i=0;i<rd;i++){
        if(buf[i]=='\n'){
            buf[i]='\0';
            return;
        }
        if (buf[i] >= 'a' && buf[i] <= 'z') buf[i]=buf[i]-32;
        else buf[i]=buf[i];
    }
    buf[i]='\0';
}

void producer(int* fPipe){
    close(fPipe[0]);
    char buf[DIM];
    strcpy(buf,"");
    int rd=1;
    while(strcmp(buf,"end\n")){
        strcpy(buf,"");
        rd=read(0,&buf[0],DIM);
        write(fPipe[1],&rd,sizeof(int));
        buf[rd]='\0';
        if(rd>0)    write(fPipe[1],&buf[0],rd);
        else break;
    }
    close(fPipe[1]);
    exit(1);
}

void consumer(int* fPipe){
    close(fPipe[1]);
    char buf[DIM];
    strcpy(buf,"");
    int rd=1,x;
    while(strcmp(buf,"END")!=0){
        strcpy(buf,"");
        read(fPipe[0],&x,sizeof(int));
        rd=read(fPipe[0],&buf[0],x);
        if(rd>0){
            createString(buf,rd);
            printf("%s\n",buf);
        }
        else break;
    }
    close(fPipe[0]);
    exit(1);
}

int main(int argc,char** argv){
    pid_t leftSon,rightSon;
    int fPipe[2];
    if(!pipe(fPipe)){
        leftSon=fork();
        if(!leftSon) producer(fPipe);
        else{
            rightSon=fork();
            if(!rightSon) consumer(fPipe);
            else{
               wait(NULL);
               wait(NULL);
               close(fPipe[1]);
               close(fPipe[0]);
            }
        }
    }
    exit(1);
}
