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

int main(int argc,char** argv){
    if(argc!=2) exit(0);
    pid_t pid;
    int f[2];
    if(!pipe(f)){
        pid=fork();
        if(!pid){
            close(f[0]);
            int fp=open(argv[1],O_RDONLY);
            int rd=1;
            char buf[DIM];
            while(rd>0){
                strcpy(buf,"");
                rd=read(fp,&buf[0],DIM);
                if(rd==(-1)){
                    printf("Errore in lettura file\n");
                    break;
                }
		        if(rd>0) write(f[1],&buf[0],rd);
	        }
            close(f[1]);
            close(fp);
            exit(1);
        }
        else{
            close(f[1]);
            int rd=1;
            char buf[DIM];
            while(rd>0){
                strcpy(buf,"");
                rd=read(f[0],&buf[0],DIM);
                if(rd==(-1)){
                    printf("Errore in lettura dalla pipe\n");
                    break;
                }
		        if(rd>0) write(1,&buf[0],rd);
	        }
            close(f[0]);
            exit(1);
        }
    }
    else exit(0);
}
    
