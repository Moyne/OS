#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char** argv){
    if(argc!=2) exit(0);
    int n=atoi(argv[1]),end=0;
    int* vet=calloc(n,sizeof(int));
    pid_t pid;
    int i=0;
    while(!end){
        pid=fork();
        if(!pid){
            for(int i=0;i<n;i++)    printf("%d",vet[i]);
            printf("\n");
            free(vet);
            exit(1);
        }
        else{
            vet[n-1]=vet[n-1]+1;
            for(int j=n-1;j>=0;j--){
                if(!j && vet[j]==2) end=1;
                if(vet[j]==2 && !end){
                    vet[j]=0;
                    vet[j-1]=vet[j-1]+1;
                }
            }
            i++;
        }
    }
    for(int j=0;j<i;j++)    wait(NULL);
    free(vet);
    exit(1);
}
