#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

typedef struct canasta{
	int numeros[3500];
	int noCanasta;
} CANASTA;


void *ordenamiento(void *arg){
	int i = 0, j = 0, k = 0;
	CANASTA *canastaP;
	canastaP = (CANASTA *)arg;

	for(i=1; i < 3500; i++){
		for(j=0; j < 3499; j++){
			if((canastaP->numeros[j])>(canastaP->numeros[j+1])){
				k = (canastaP->numeros[j]);
				(canastaP->numeros[j]) = (canastaP->numeros[j+1]);
				(canastaP->numeros[j+1]) = k;
			}
		}
	}
	
	pthread_exit(NULL);
}

void ImprimirCanastas(int numeros[3500], CANASTA *canastas, int noHilos){
  int i,j;

  for(i=0; i < noHilos; i++){
  	printf("Números de la canasta %d:\n", i);
  	for(j=0; j < 3500; j++){
		if((*(canastas+i)).numeros[j]!=-1){
	  		printf("%d\t", (*(canastas+i)).numeros[j]);
		}
  	}
	printf("\n\n");
  }
}

void escribirArchivoOrdenado(CANASTA *canastas, int noHilos){
	FILE* fp;
	int i,
			j;
	fp=fopen("Numeros.txt","w");
	if(fp==NULL)
		exit(-1);

	fprintf(fp, "\nLas canastas ordenadas son:\n");
	for(i=0; i<noHilos; i++){
		fprintf(fp, "Números de la canasta %d:\n", i);
		for(j=0; j<3500; j++){
			if((*(canastas+i)).numeros[j]!=-1){
				fprintf(fp, "%d\t", (*(canastas+i)).numeros[j]);
			}
		}
		fprintf(fp, "\n\n");
	}
	fclose(fp);
}


int main() {
  	srand(time(NULL));
  
  	int numeros[3500], i, j, noHilos=0, rango=0, rangoPrincipal=0, rangoAnterior=0, *contadores, error=0;
  
	CANASTA *canastas=NULL;
	pthread_t *hilos;

	
	for(i=0; i < 3500; i++){
		numeros[i]= rand()%1000;
	}

	printf("Cantidad de hilos: ");
	scanf("%d", &noHilos);
	
	rango=1000/noHilos;
	rangoPrincipal=rango;
	
	hilos = (pthread_t *) malloc(noHilos*sizeof(pthread_t));
	canastas = (CANASTA *) malloc(noHilos*sizeof(CANASTA));
	contadores = (int *) malloc(noHilos*sizeof(int));

	printf("\n\n");

	for(i=0; i < noHilos; i++){
		*(contadores+i)=0;
			(*(canastas+i)).noCanasta=i;
			printf("Canasta: %d\n", i);
			printf("rangos %d - %d\n", rangoAnterior, rango);
		for(j=0; j < 3500; j++){
			(*(canastas+i)).numeros[j]=-1;
			if(numeros[j]<rango && numeros[j]>=rangoAnterior){
				(*(canastas+i)).numeros[(*(contadores+i))]=numeros[j];
				(*(contadores+i))++;
			}
		}
			rangoAnterior=rango;
			rango+=rangoPrincipal;
	}

	printf("\n\n");

		
	ImprimirCanastas(numeros, canastas, noHilos);

		
	for(i=0; i < noHilos; i++){

		error = pthread_create((hilos+i), NULL, ordenamiento, (canastas+i));
		if(error){
			fprintf(stderr,"Error %d\n", error);
			exit(-1);
		}
	}

	for(i=0; i < noHilos; i++){
		error=pthread_join(*(hilos+i), NULL);
		if(error){
			fprintf(stderr,"Error %d\n", error);
		}
		else{
			printf("Hilo %d terminado\n", i);
		}
	}

		
	escribirArchivoOrdenado(canastas, noHilos);
	
	return 0;
}
