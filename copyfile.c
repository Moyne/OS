#include <stdio.h>
#define DIM 50
int main(int argc,char** argv){
	if(argc<3) return 0;
	FILE* fp;
	FILE* fr;
	fp=fopen(argv[1],"r");
	fr=fopen(argv[2],"w");
	if(fp==NULL || fr==NULL) return 0;
	char buf [DIM];
	while(fread(&buf[0],sizeof(char),DIM,fp)>0){
		fwrite(&buf[0],sizeof(char),DIM,fr);
	}
	fclose(fp);
	fclose(fr);
	return 0;
}	
