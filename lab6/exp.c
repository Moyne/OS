#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#define DIM 50

int main(int argc,char** argv){
    if(argc!=2) exit(0);
    int n=atoi(argv[1]);
    if(n>10)    n=10;
    pid_t pid;
    int f[2];
    if(!pipe(f)){
        for(int i=1;i<n;i++){
            pid=fork();
            if(!pid){
                close(f[0]);
                int x= (i+7)/i;
                write(f[1],&x,sizeof(int));
                close(f[1]);
                exit(1);
            }
        }
        close(f[1]);
        int x=0,y;
        for(int i=1;i<n;i++){
            if(!read(f[0],&y,sizeof(int)))   break;
            x+=y;
        }
        for(int i=1;i<n;i++)    wait(NULL);
        close(f[0]);
        printf("The result with n.%d is %d\n",n,x);
    }
    exit(0);
}
