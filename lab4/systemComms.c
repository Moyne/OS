#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#define DIM 80
int main(int argc,char** argv){
    if(argc!=2) exit(0);
    FILE* fp;
    pid_t pid;
    fp=fopen(argv[1],"r");
    char buffer[DIM];
    while(fgets(buffer,DIM,fp)!=NULL){
        char* comm=malloc((strlen(buffer)-4)*sizeof(char));
        for(int i=0;i<strlen(buffer)-5;i++) comm[i]=buffer[i];
        comm[strlen(buffer)-5]='\0';
        system(comm);
        free(comm);
        sleep(3);
    }
    fclose(fp);
    exit(0);
}
