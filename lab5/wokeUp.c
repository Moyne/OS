#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void handler(int sig){  if(sig==SIGCHLD) exit(0); }

int main(int argc,char** argv){
    if(argc!=2) exit(0);    
    pid_t pid;
    int n=atoi(argv[1]);
    int i=0;
    pid=fork();
    if(!pid){
        signal(SIGUSR1,handler);
        while(i<n){
            printf("Child woke up\n");
            sleep(1);
            kill(getppid(),SIGUSR1);
            pause();
            i++;
        }
        exit(1);
    }
    else{
        printf("My child has pid=%d\n",pid);
        signal(SIGUSR1,handler);
        signal(SIGCHLD,handler);
        while(1){
            pause();
            printf("Father woke up\n");
            sleep(1);
            kill(pid,SIGUSR1);
        }
    }
    exit(1);
}
