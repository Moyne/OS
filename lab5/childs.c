#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DIM 50

void childHandler(int sig){
    if(sig==SIGCHLD)    printf("E' terminato uno dei processi figli!\n");
    else    printf("Catturato un segnale errato!!!");
    return;
}

void sonProcess(int son){
    printf("Processo n. %d\n",getpid());
    char fileName[DIM];
    char buf[DIM+5];
    if(son==1) strcpy(fileName,"son1.txt");
    else if(son==2) strcpy(fileName,"son2.txt");
    int fp=open(fileName,O_RDONLY);
    if(fp==(-1)){
        printf("Errore apertura del file");
        exit(0);
    }
    read(fp,&buf[0],DIM);
    printf("I primi 50 byte del file %s sono:%s\n",fileName,buf);
    if(son==2)  sleep(5);
    close(fp);
    exit(1);
}    
int main(int argc,char** argv){
    signal(SIGCHLD,childHandler);
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
            pause();
            exit(1);
        }
    }
}
