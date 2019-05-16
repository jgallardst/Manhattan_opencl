#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

void initialize(int *m,int t,int maximo)
{
  int i;
  for(i=0;i<t*t;i++)
  {  
    m[i] =(int) (((1.*rand())/RAND_MAX)*maximo);
  }
}

void escribir(int *m, int t){
  int i, j;

  for (i = 0; i < t; i++)
  {
    for (j = 0; j < t; j++)
    {  
      printf("%d ",m[i*t+j]);
    }
    printf("\n");
  }
  printf("\n");
}

void escribirresult(int *m, int t){
  int i;

  for (i = 0; i < t; i++)
  {
    printf("%d ",m[i]);
  }
  printf("\n");
}

/*
c
c     mseconds - returns elapsed milliseconds since Jan 1st, 1970.
c
*/
long long mseconds(){
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec*1000 + t.tv_usec/1000;
}

int ObtenerParametros(int argc, char *argv[], int *debug) {
	int i;
	*debug=0;
	if (argc<2)
		return 0;
	for (i=2; i<argc;) {
		if (strcmp(argv[i], "-d")==0) {
			*debug=1;
			i++;
		}
		else
			return 0;
	}
	return 1;
}

// **************************************************************************
// ***************************** IMPLEMENTACI�N *****************************
// **************************************************************************
/*
N -> Tama�o de la matriz A (NxN)
A -> Matriz
n -> Cantidad de enteros contenidos en numeros
numeros -> Lista de n�meros de los que se buscar�n las distancias
distancias -> Resultado con las distancias (un resultado por n�mero)
*/

void sec(int N,int *A,int n,int *numeros,int *distancias) {
	for (int num = 0; num < n; num++){
		int seek = numeros[num];
		int hits = 0;
		int *pointer = (int*) malloc(hits * sizeof(int));
		int max = 0;
		for (int i = 0; i < N*N; i++){
			if(A[i] == seek){
				hits++;
				pointer = (int*)realloc(pointer, hits * sizeof(int));
				pointer[hits - 1] = i;
			}
		}
		for (int i = 0; i < hits-1; i++){
			int source = pointer[i];
			for (int j = i+1; j < hits; j++){
				int dest = pointer[j];
				int dist = abs(source / N - dest / N) + abs((source % N) - (dest % N));
				if (dist > max) max = dist;
			}
		}
		distancias[num] = max;
	}
}

// **************************************************************************
// *************************** FIN IMPLEMENTACI�N ***************************
// **************************************************************************

/*
Recibir� los siguientes par�metros (los par�metros entre corchetes son opcionales): fichEntrada [-d]
fichEntrada -> Obligatorio. Fichero de entrada con los par�metros de lanzamiento de los experimentos
-d -> Opcional. Si se indica, se mostrar�n por pantalla los valores iniciales, finales y tiempo de cada experimento
*/
int main(int argc,char *argv[]) {
	int i,j,
		debug=0,		  // Indica si se desean mostrar los tiempos y resultados parciales de los experimentos
		N,			  // Tama�o de la matriz A (NxN)
		cuantos,		  // N�mero de experimentos
		n,			  // N�mero de enteros a leer del fichero
		semilla,		  // Semilla para la generaci�n de n�meros aleatorios
		maximo,		  // Valor m�ximo de para la generaci�n de valores aleatorios de la matriz A (se generan entre 0 y maximo-1)
		*A,			  // Matriz A. Se representa en forma de vector. Para acceder a la fila f y la columna c: A[f*N+c]
		*numeros,		  // Vector que contiene los n�meros de los que se calcular�n las distancias
		*distancias;	  // Vector resultado conteniendo las distancias calculadas
	long long ti,		  // Tiempo inicial
			tf,		  // Tiempo final
			tt=0; 	  // Tiempo acumulado de los tiempos parciales de todos los experimentos realizados
	FILE *f;			  // Fichero con los datos de entrada
	
	if (!ObtenerParametros(argc, argv, &debug)) {
		printf("Ejecuci�n incorrecta\nEl formato correcto es %s fichEntrada [-d]\n", argv[0]);
		return 0;
	}
	
	// Se leen el n�mero de experimentos a realizar
	f=fopen(argv[1],"r");
	fscanf(f, "%d",&cuantos);

	for(i=0;i<cuantos;i++)
	{
		//Por cada experimento se leen
		fscanf(f, "%d",&N);			//El tama�o de la matriz (NxN)
		fscanf(f, "%d",&semilla);	//La semilla para la generaci�n de n�meros aleatorios
		fscanf(f, "%d",&maximo);		//El valor m�ximo de para la generaci�n de valores aleatorios de la matriz A (se generan entre 0 y maximo-1)
		fscanf(f, "%d",&n);			//El n�mero de enteros a leer del fichero
		//Reserva de memoria para la matriz, las distancias y los n�meros
		A = (int *) malloc(sizeof(int)*N*N);
		distancias=(int *) malloc(sizeof(int)*n);
		numeros = (int *) malloc(sizeof(int)*n);
		//Lectura de los n�meros
		for(j=0;j<n;j++) {
			fscanf(f, "%d",&numeros[j]);
		}

		srand(semilla);
		initialize(A,N,maximo);

		if (debug)	{
			printf("Matriz del experimento %d:\n", i); escribir(A,N);
			printf("N�meros del experimento %d:\n", i);escribirresult(numeros,n);
		}
   
		ti=mseconds(); 
		sec(N,A,n,numeros,distancias);
		tf=mseconds(); 
		tt+=tf-ti;

		printf("Tiempo del experimento %d: %Ld ms\n", i, tf-ti);

		if (debug)	{
			printf("Resultado del experimento %d:\n", i); escribirresult(distancias,n);
		}

		free(A);
		free(numeros);
		free(distancias);
	}
	printf("Tiempo total de %d experimentos: %Ld ms\n", cuantos, tt);
	fclose(f);

  return 0;
}
