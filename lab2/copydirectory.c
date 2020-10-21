#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define DIM 257
#define DIMFILE 50
int copyFile(char* src,char* dest){
    int fp,fr;
    fp=open(src,O_RDONLY);
    fr=open(dest,O_WRONLY | O_CREAT | O_SYNC | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if((fp==(-1)) || (fr==(-1))) return -1;
    char buf [DIMFILE];
    int rd=1;
    while(rd>0){
        rd=read(fp,&buf[0],DIMFILE);
        if(rd==(-1)){
            close(fr);
            close(fp);
            return -1;
        }
		if(rd>0) write(fr,&buf[0],rd);
	}
    close(fp);
    close(fr);
    return 1;
}
int openEntry(char* path,char* dest,int* level){
    for(int i=0;i<(*level);i++) printf("\t");
    printf("%s\n",path);
    struct stat entry;
    if(stat(path,&entry)<0) return -1;
    char name[DIM];
    char entryPath[DIM];
    char destPath[DIM];
    if(S_ISDIR(entry.st_mode)){
        if(mkdir(dest,S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP |S_IXGRP | S_IROTH | S_IXOTH)==(-1)) return -1;
        DIR *dp;
        if((dp=opendir(path))!=NULL){
            struct dirent *dirp;
            while((dirp=readdir(dp))!=NULL){
                if(strcmp(dirp->d_name,".")!=0 && strcmp(dirp->d_name,"..")!=0) {
                    strcpy(entryPath,path);
                    if(entryPath[strlen(entryPath)-1]!='/')   strcat(entryPath,"/");
                    strcat(entryPath,dirp->d_name);
                    strcpy(destPath,dest);
                    if(destPath[strlen(destPath)-1]!='/')   strcat(destPath,"/");
                    strcat(destPath,dirp->d_name);
                    (*level)++;
                    if(openEntry(entryPath,destPath,level)==(-1)) return -1;
                    (*level)--;
                }
            }
            if(closedir(dp)<0) return -1;
        }
    }
    else    if(copyFile(path,dest)==(-1)) return -1;
    return 1;
}
            
int main(int argc,char** argv){
    if(argc!=3) return 0;
    int level=0;
    char dest [DIM];
    strcpy(dest,argv[2]);
    strcat(dest,strrchr(argv[1],'/'));
    printf("Destinazione copia : %s",dest);
    openEntry(argv[1],dest,&level);
    return 1;
}
