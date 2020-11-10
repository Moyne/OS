#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char** argv){
	if(argc!=3){
		printf("Errore:	numero errato di parametri inserito!!\n");
		exit(1);
	}
	if(!strcmp(argv[2],"somma"))	kill(atoi(argv[1]),SIGUSR1);
	else if(!strcmp(argv[2],"differenza"))	kill(atoi(argv[1]),SIGUSR2);
	else if(!strcmp(argv[2],"fine"))	kill(atoi(argv[1]),SIGINT);
	else	fprintf(stderr,"Errore:	parametro comando errato\n");
	exit(1);
}
