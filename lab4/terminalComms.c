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
    char path[DIM];
    while(fgets(buffer,DIM,fp)!=NULL){
        printf("\n");
        strcpy(path,"/bin/");
        int lenght=strlen(buffer);
        int i=1,l=0,h=0,j=0;
        for(int j=0;j<lenght;j++) if(buffer[j]==' ') i++;
        //I CREATE PARAM** THAT HAS I STRINGS, I BECAUSE I NEED I-1 PARAMETERS AND THE Ith ELEMENT OF THE VECTOR WILL BE (CHAR*) 0 TO PASS TO EXECV
        char** param=malloc(i*sizeof(char*));
        for(int j=0;j<i-1;j++) param[j]=malloc(DIM*sizeof(char));
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
        strcat(path,param[0]);
        printf("\tCommand :%s",buffer);
        pid=fork();
        if(!pid){
            if(i>2) execv(path,param);
            else execl(path,param[0],(char*) 0);
        }
        wait(NULL);
        sleep(3);
        for(int j=0;j<i-1;j++) free(param[j]);
        free(param);
    }
    fclose(fp);
    exit(0);
}
