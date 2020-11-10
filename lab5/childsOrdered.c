#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DIM 50

void handler(int sig)   {  return; }

void sonProcess(int son){
    signal(SIGUSR2,handler);
    signal(SIGUSR1,(void(*)())0);
    signal(SIGCHLD,(void(*)())0);
    if(son==1) pause();
    printf("Processo n. %d\n",getpid());
    char fileName[DIM];
    char buf[DIM+2];
    if(son==1) strcpy(fileName,"son1.txt");
    else if(son==2) strcpy(fileName,"son2.txt");
    int fp=open(fileName,O_RDONLY);
    if(fp==(-1)){
        printf("Errore apertura del file");
        exit(0);
    }
    int rd=1;
    int i=0;
    while(rd>0){
        strcpy(buf,"");
        rd=read(fp,&buf[0],DIM);
        if(rd==(-1)){
            printf("Errore in lettura file\n");
            break;
        }
		if(rd>0) write(1,&buf[0],rd);
	}
    close(fp);
    kill(getppid(),SIGUSR1);
    exit(1);
}    
int main(int argc,char** argv){
    signal(SIGCHLD,handler);
    signal(SIGUSR1,handler);
    pid_t leftSon,rightSon;
    int sonNumber=0;
    leftSon=fork();
    sonNumber++;
    if(leftSon==0)  sonProcess(sonNumber);
    else{
        rightSon=fork();
        sonNumber++;
        if(rightSon==0)  sonProcess(sonNumber);
        else{
            pause();
            kill(leftSon,SIGUSR2);
            pause();
            exit(1);
        }
    }
}
