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
    // WITH EXECV I NEED THE PATH char path[DIM];
    while(fgets(buffer,DIM,fp)!=NULL){
        printf("\n");
        // WITH EXECV I NEED THE PATH strcpy(path,"/bin/");
        int lenght=strlen(buffer);
        int i=1,l=0,h=0,j=0;
        for(int j=0;j<lenght;j++) if(buffer[j]==' ') i++;
        //I CREATE PARAM** THAT HAS I STRINGS, I BECAUSE I NEED I-1 PARAMETERS AND THE Ith ELEMENT OF THE VECTOR WILL BE (CHAR*) 0 TO PASS TO EXECV
        char** param=malloc(i*sizeof(char*));
        for(int j=0;j<i-1;j++)  param[j]=malloc(DIM*sizeof(char));
        //I SEARCH FOR THE PARAMETERS NEEDED AND I SAVE THEM IN THE DYNAMIC ALLOCATED PARAM**
        while(l<(i-1) && j<lenght){
            if(buffer[j]==' '){
                param[l][h]='\0';   //I FOUND A NEW SPACE SO I NEED TO END THE OLD STRING WITH \0
                h=0;
                l++;
            }
            else{
                param[l][h]=buffer[j];
                h++;
            }
            j++;
        }
        param[i-1]=(char*)0;
        // WITH EXECV I NEED THE PATH strcat(path,param[0]);
        printf("\tCommand :%s",buffer);
        pid=fork();
        if(!pid)    execvp(param[0],param); //EXECVP SO I CAN CALL ALSO NEOFETCH AND OTHER PROGRAMS NOT IN /BIN/, OTHERWISE EXECV(PATH,PARAM[0],PARAM);
        wait(NULL);
        sleep(3);
        for(int j=0;j<i-1;j++) free(param[j]);
        free(param);
    }
    fclose(fp);
    exit(0);
}
