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

pid_t* childsPids;
int lenght;
int counter;
void childsHandler(int sig){    return; }

void fatherHandler(int sig){
    counter++;
    if(counter<lenght)  kill(childsPids[counter],SIGUSR1);
}

void son(int pos,char* fileName){
    signal(SIGUSR1,childsHandler);
    int readAgain=1;
    int fp=open(fileName,O_RDWR | O_SYNC);
    int x,y,rd,z=pos+1;
    while(readAgain!=0){
        lseek(fp,2*pos*sizeof(int),SEEK_SET);
        rd=read(fp,&x,sizeof(int));
        if(x==pos) readAgain=0;
        else pause();
    }
    rd=read(fp,&x,sizeof(int));
    if(rd==-1){close(fp);exit(1);}
    read(fp,&rd,sizeof(int));
    rd=read(fp,&y,sizeof(int));
    if(rd==-1){close(fp);exit(1);}
    if(x>y){
        lseek(fp,((2*pos)+1)*sizeof(int),SEEK_SET);
        write(fp,&y,sizeof(int));
        write(fp,&pos,sizeof(int));
        write(fp,&x,sizeof(int));
    }
    lseek(fp,2*(pos+1)*sizeof(int),SEEK_SET);
    write(fp,&z,sizeof(int));
    kill(getppid(),SIGUSR1);
    close(fp);
    exit(1);
}

int main(int argc,char** argv){
    if(argc!=3) exit(1);
    signal(SIGUSR1,fatherHandler);
    pid_t pid;
    int n=atoi(argv[2]);
    int fp=open(argv[1],O_RDWR | O_CREAT | O_SYNC | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    int x,z=0;
    for(int i=0;i<n;i++){
        printf("Write number %d of the array: ",i);
        scanf("%d",&x);
        write(fp,&z,sizeof(int));
        write(fp,&x,sizeof(x));
    }
    printf("File prima del bubble sort\n");
    lseek(fp,0,SEEK_SET);
    for(int i=0;i<n;i++){
        read(fp,&x,sizeof(int));
        read(fp,&x,sizeof(int));
        printf("%d ",x);
    }
    printf("\n");
    for(int i=0;i<n-1;i++){
        childsPids=malloc((n-1-i)*(sizeof(pid_t)));
        lenght=n-1-i;
        counter=0;
        for(int j=0;j<n-1-i;j++){
            pid=fork();
            if(!pid)    son(j,argv[1]);
            childsPids[j]=pid;
        }
        for(int j=0;j<n-1-i;j++) wait(NULL);
        lseek(fp,0,SEEK_SET);
        for(int i=0;i<n;i++){
            write(fp,&z,sizeof(int));
            read(fp,&x,sizeof(int));
        }
        free(childsPids);
    }
    printf("File dopo il bubble sort\n");
    lseek(fp,0,SEEK_SET);
    for(int i=0;i<n;i++){
        read(fp,&x,sizeof(int));
        read(fp,&x,sizeof(int));
        printf("%d ",x);
    }
    printf("\n");
    close(fp);
    exit(1);
}
