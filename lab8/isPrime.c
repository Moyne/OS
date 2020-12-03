#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

typedef struct{
    int start,end,val;
}param;

void *thread(void* f){
    param* params=f;
    int start=params->start,end=params->end,val=params->val;
    for(int i=start;i<=end;i++){
        float res=(float) val/i;
        int resInt=(int) res;
        if(res==resInt){
            params->val=0;
            pthread_exit(&f);
        }
    }
    params->val=1;
    pthread_exit(&f);
}

int main(int argc,char** argv){
	if(argc!=3)	exit(0);
	int n=atoi(argv[1]);
	int nP=atoi(argv[2]);
	float y=(float) ((n/2-2)+1)/nP;
    int x= (int) y;
    float zFloat=y-(float)x;
    float z=nP*zFloat;
    int inc=(int) (z+0.00001);
    int endProg=0,nPrime=0,stoppedAt=nP;
    pthread_t* threads=malloc(nP*sizeof(pthread_t));
    param* params=malloc(nP*sizeof(param));
    int start=2,end;
    for(int i=0;i<nP;i++){
        if(i<inc) end=start+x;
        else end=start+x-1;
        params[i].start=start;  params[i].end=end;  params[i].val=n;
        int rc;
        for(int j=0;j<i;j++){
            if(!params[j].val){
                endProg=1;
                stoppedAt=i;
                break;
            }
        }
        if(!endProg){
            rc=pthread_create(&threads[i],NULL,thread,(void*) &params[i]);
            if(rc)  exit(0);
            start=end+1;
        }
        else break;
    }
    
    while(!endProg){
        nPrime=0;
        for(int i=0;i<nP;i++){
            if(!params[i].val){
                endProg=1;
                break;
            }
            if(params[i].val==1) nPrime++;
        }
        if(nPrime==nP)  break;
    }
    for(int i=0;i<stoppedAt;i++)    pthread_cancel(threads[i]);
    free(params);
    free(threads);
    if(!endProg)    printf("The number %d is a prime number!!\n",n);
    else printf("The number %d is not a prime number!!\n",n);
    exit(1);
}
    
