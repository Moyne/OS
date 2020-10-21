#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#define DIM 50
int main(int argc,char** argv){
    if(argc!=3) return 0;
    int fp,fr;
    fp=open(argv[1],O_RDONLY);
    fr=open(argv[2],O_WRONLY | O_CREAT | O_SYNC | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if((fp==(-1)) || (fr==(-1))) return 0;
    char buf [DIM];
    int rd=1;
    while(rd>0){
        rd=read(fp,&buf[0],DIM);
		if(rd>0) write(fr,&buf[0],rd);
	}
    close(fp);
    close(fr);
    return 0;
}
