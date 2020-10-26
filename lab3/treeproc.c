#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc,char** argv){
	int level,time,n;
	if(argc<3 || argc>4) exit(0);
	if(argc==3) level=0;
	else if(argc==4) level=atoi(argv[3]);
	n=atoi(argv[1]);
	time=atoi(argv[2]);
	if(n==level){
        sleep(time);
	    printf("\nThis process is a leaf, pid: %d , parent pid: %d\n",getpid(),getppid());
		exit(0);
	}
	else if(level==0)   printf("\nThis process is at the level: %d ,and has pid: %d\n",level,getpid());
    else if(level>0){
        sleep(time);
        printf("\nThis process is at the level: %d ,and has pid: %d ,parent pid: %d\n",level,getpid(),getppid());
    }
	level++;
	pid_t leftSon,rightSon;
	leftSon=fork();
	char levelS[20];
	sprintf(levelS,"%d",level);
	if(leftSon==0) execl("./treeproc","treeProc",argv[1],argv[2],levelS,(char *)0);
	else{
		rightSon=fork();
		if(rightSon==0) execl("./treeproc","treeProc",argv[1],argv[2],levelS,(char *)0);
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

