#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DIM 50

void handler(int sig){  return; }

void createString(char* buf,int rd){
    char tmp[DIM];
    strcpy(tmp,buf);
    int i,j;
    for(i=0,j=rd-1;i<rd && j>=0;i++,j--){
        if (tmp[j] >= 'a' && tmp[j] <= 'z') buf[i]=tmp[j]-32;
        else buf[i]=tmp[j];
    }
    buf[i]='\0';
}

void producer(pid_t brother,char* fileName){
    signal(SIGUSR1,handler);
    pause();
    sleep(1);
    int fp=open(fileName,O_RDWR | O_SYNC);
    char buf[DIM];
    read(fp,&buf[0],DIM);
    brother=atoi(buf);
    printf("Sono il processo n.%d e il mio fratello e' il processo n.%d\n",getpid(),brother);
    strcpy(buf,"");
    int rd=1;
    while(strcmp(buf,"end\n")){
        printf("Write: \n");
        strcpy(buf,"");
        rd=read(0,&buf[0],DIM);
        buf[rd]='\0';
        if(rd>0){
            write(fp,&buf[0],rd);
            kill(brother,SIGUSR1);
            pause();
        }
        else break;
    }
    close(fp);
    exit(1);
}

void consumer(pid_t brother,char* fileName){
    signal(SIGUSR1,handler);
    int fp=open(fileName,O_RDWR | O_CREAT | O_SYNC | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    printf("Sono il processo n.%d e il mio fratello e' il processo n.%d\n",getpid(),brother);
    char buf[DIM];
    sprintf(buf,"%d",getpid());
    write(fp,&buf[0],sizeof(int));
    strcpy(buf,"");
    int rd=1;
    kill(brother,SIGUSR1);
    while(strcmp(buf,"\nDNE")!=0){
        pause();
        strcpy(buf,"");
        rd=read(fp,&buf[0],DIM);
        if(rd>0){
            createString(buf,rd);
            printf("%s\n",buf);
            kill(brother,SIGUSR1);
        }
        else break;
    }
    close(fp);
    exit(1);
}

int main(int argc,char** argv){
    pid_t leftSon,rightSon;
    leftSon=fork();
    if(!leftSon) producer(0,argv[1]);
    else{
        rightSon=fork();
        if(!rightSon) consumer(leftSon,argv[1]);
        else{
           wait(NULL);
           wait(NULL);
        }
    }
    exit(1);
}
                
