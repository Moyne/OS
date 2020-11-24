#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DIM 50

int** mat;

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void *thread(void* f){
    char d[DIM];
    strcpy(d,(char *)f);
    int index=atoi(strchr(d,' '));
    int k,j=0;
    char fileName[DIM];
    for(k=0;k<strlen(d);k++){  
        if(d[k]==' '){
            fileName[k]='\0';
            break;
        }
        fileName[k]=d[k];
    }
    FILE* fIn;
    fIn=fopen(fileName,"r");
    if(fIn==NULL){
        fprintf(stderr,"Error in the'opening of the file!\n");
        pthread_exit(f);
    }
    while(fscanf(fIn,"%d",&mat[index][j])==1)   j++;
    mergeSort(mat[index],0,j-1);
    fclose(fIn);
    pthread_exit(f);
}
       
int main(int argc,char** argv){
    if(argc<5) exit(0);
    int n=atoi(argv[1]);
    int m=atoi(argv[2]);
    if(argc!=(3+n+1))   exit(0);
    mat=malloc(n*sizeof(int*));
    pthread_t* tids=malloc(n*sizeof(pthread_t));
    char** params=malloc(n*sizeof(char*));
    for(int i=0;i<n;i++) mat[i]=malloc(m*sizeof(int));
    for(int i=0;i<n;i++){
        params[i]=malloc(DIM);
        strcpy(params[i],argv[i+3]);
        char d[5];
        sprintf(d," %d",i);
        strcat(params[i],d);
        int rc=pthread_create(&tids[i],NULL,thread,(void *) params[i]);
        if(rc)  exit(0);
    }
    void* status;
    int* vetRis=malloc(m*sizeof(int));
    for(int i=0;i<n;i++){
        pthread_join(tids[i],&status);
        free(params[i]);
        if(i>0) vetRis=realloc(vetRis,(i+1)*m*sizeof(int));
        for(int j=0;j<m;j++)    vetRis[(i*m)+j]=mat[i][j];
        if(i>0) merge(vetRis,0,(i*m)-1,((i+1)*m)-1);
        free(mat[i]);
    }
    free(params);
    free(mat);
    int fRis=open(argv[argc-1],O_WRONLY | O_CREAT | O_SYNC | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if(fRis==(-1)) return -1;
    int rd=1;
    printf("Result:\n");
    for(int i=0;i<m*n;i++) printf("%d ",vetRis[i]);
    rd=write(fRis,vetRis,m*n*sizeof(int));
    if(rd==(-1))    fprintf(stderr,"Error in the writing of the file!\n");
    close(fRis);
    printf("\n");
    free(vetRis);
    exit(0);
}
    
