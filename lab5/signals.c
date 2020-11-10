#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int a,b;
void signalHandler(int sig){
	if(sig==SIGUSR1)	printf("La somma e' uguale a: %d\n",a+b);
	else if(sig==SIGUSR2)	printf("La differenza e' uguale a : %d\n",a-b);
	else if(sig==SIGINT){
        printf("Ricevuto segnale di terminazione del processo SIGINT!\n");
        exit(1);
    }
	else	printf("Errore, segnale sbagliato ricevuto!!\n");
	return;
}

int main(int argc,char** argv){
	printf("Processo con pid= %d\n",getpid());
	printf("Scrivi il numero a: ");
	scanf("%d",&a);
	printf("Scrivi il numero b: ");
	scanf("%d",&b);
	signal(SIGUSR1,signalHandler);
	signal(SIGUSR2,signalHandler);
	signal(SIGINT,signalHandler);
	while(1) pause();
	exit(1);
}
