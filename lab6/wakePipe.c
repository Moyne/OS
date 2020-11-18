#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DIM 50

int main(int argc,char** argv){
    if(argc!=2) exit(0);    
    pid_t pid;
    int n=atoi(argv[1]);
    int i=0;
    char buf[DIM];
    int fPipeF[2],fPipeS[2];
    if(!pipe(fPipeF) && !pipe(fPipeS)){
        pid=fork();
        if(!pid){
            close(fPipeF[1]);
            close(fPipeS[0]);
            while(1){
                strcpy(buf,"");
                read(fPipeF[0],&buf[0],DIM);
                if(strcmp(buf,"Father end")) break;
                else{
                    printf("I'm the child (PID=%d)\n",getpid());
                    strcpy(buf,"Child end");
                    write(fPipeS[1],&buf[0],DIM);
                }
            }
            close(fPipeS[1]);
            close(fPipeF[0]);
            exit(1);
        }
        else{
            close(fPipeS[1]);
            close(fPipeF[0]);
            while(i<n){
                printf("I'm the father (PID=%d i=%d)\n",getpid(),i);
                strcpy(buf,"Father end");
                write(fPipeF[1],&buf[0],DIM);
                strcpy(buf,"");
                read(fPipeS[0],&buf[0],DIM);
                if(strcmp(buf,"Child end")) break;
                else i++;
            }
            strcpy(buf,"Program end");
            write(fPipeF[1],&buf[0],DIM);
            wait(NULL);
            close(fPipeF[1]);
            close(fPipeS[0]);
        }
    }
    exit(1);
}
