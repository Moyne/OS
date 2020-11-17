#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#define DIM 50

void childHandler(int sig){ return; }

int main(int argc,char** argv){
    if(argc!=2) exit(0);
    int n=atoi(argv[1]);
    if(n>10)    n=10;
    pid_t pid;
    int f[2];
    if(!pipe(f)){
        pid_t *childsPids=malloc((n-1)*sizeof(pid_t));
        for(int i=1;i<n;i++){
            pid=fork();
            if(!pid){
                signal(SIGUSR1,childHandler);
                close(f[0]);
                int x= (i+7)/i;
                write(f[1],&x,sizeof(int));
                pause();
                close(f[1]);
                exit(1);
            }
            else childsPids[i-1]=pid;
        }
        close(f[1]);
        int x=0,y;
        for(int i=1;i<n;i++){
            if(!read(f[0],&y,sizeof(int)))   break;
            x+=y;
        }
        for(int i=0;i<n-1;i++)  kill(childsPids[i],SIGUSR1);
        for(int i=1;i<n;i++)    waitpid(childsPids[i],&y,0);
        close(f[0]);
        free(childsPids);
        printf("The result with n.%d is %d\n",n,x);
    }
    exit(0);
}
