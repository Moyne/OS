#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DIM 257
int openEntry(char* path,int* level){
    for(int i=0;i<(*level);i++) printf("\t");
    printf("%s\n",path);
    struct stat entry;
    if(stat(path,&entry)<0) return -1;
    char name[DIM];
    char entryPath[DIM];
    if(S_ISDIR(entry.st_mode)){
        DIR *dp;
        if((dp=opendir(path))!=NULL){
            struct dirent *dirp;
            while((dirp=readdir(dp))!=NULL){
                if(strcmp(dirp->d_name,".")!=0 && strcmp(dirp->d_name,"..")!=0) {
                    strcpy(entryPath,path);
                    if(entryPath[strlen(entryPath)-1]!='/')   strcat(entryPath,"/");
                    strcat(entryPath,dirp->d_name);
                    (*level)++;
                    if(openEntry(entryPath,level)==(-1)) return -1;
                    (*level)--;
                }
            }
            if(closedir(dp)<0) return -1;
        }
    }
    return 1;
}
            
int main(int argc,char** argv){
    if(argc!=2) return 0;
    int level=0;
    openEntry(argv[1],&level);
    return 1;
}
