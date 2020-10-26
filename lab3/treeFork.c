#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
void tree(int n,int time,int *level){
    if(n==(*level)){
        sleep(time);
	    printf("\nThis process is a leaf, pid: %d , parent pid: %d\n",getpid(),getppid());
		exit(0);
	}
	else if((*level)==0)   printf("\nThis process has pid: %d\n",getpid());
    else if((*level)>0){
        sleep(time);
        printf("\nThis process is at the level: %d ,and has pid: %d ,parent pid: %d\n",(*level),getpid(),getppid());
    }
    pid_t leftSon,rightSon;
    (*level)++;
	leftSon=fork();
	if(leftSon==0) tree(n,time,level);
	else{
		rightSon=fork();
		if(rightSon==0) tree(n,time,level);
		else{
			pid_t waitRes;
			waitRes=wait(NULL);
			while(waitRes!=leftSon && waitRes!=rightSon) waitRes=wait(NULL);
			waitRes=wait(NULL);
			while(waitRes!=leftSon && waitRes!=rightSon) waitRes=wait(NULL);
		}
	}
	exit(0);
}
int main(int argc,char** argv){
	int level,time,n;
	level=0;
	n=atoi(argv[1]);
	time=atoi(argv[2]);
    tree(n,time,&level);
}
