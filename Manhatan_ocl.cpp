#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <CL/cl.h>

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

int ObtenerParametros(int argc, char *argv[], int *debug, int *num_workitems, int *workitems_por_workgroups) {
	int i;
	*debug=0;
	*num_workitems=0;
	*workitems_por_workgroups=0;
	if (argc<2)
		return 0;
	for (i=2; i<argc;) {
		if (strcmp(argv[i], "-d")==0) {
			*debug=1;
			i++;
		}
		else if (strcmp(argv[i], "-wi")==0) {
			i++;
			if (i==argc)
				return 0;
			*num_workitems=atoi(argv[i]);
			i++;
			if (*num_workitems<=0)
				return 0;
		}
		else if (strcmp(argv[i], "-wi_wg")==0) {
			i++;
			if (i==argc)
				return 0;
			*workitems_por_workgroups=atoi(argv[i]);
			i++;
			if (*workitems_por_workgroups<=0)
				return 0;
		}
		else
			return 0;
	}
	return 1;
}

typedef struct {
	cl_platform_id *plataformas;
	cl_device_id *dispositivos;
	cl_context contexto;
	cl_command_queue cola;
	cl_program programa;
	cl_kernel kernel;
} EntornoOCL_t;

// **************************************************************************
// ***************************** IMPLEMENTACI�N *****************************
// **************************************************************************
cl_int InicializarEntornoOCL(EntornoOCL_t *entorno) {

}

cl_int LiberarEntornoOCL(EntornoOCL_t *entorno) {

}

/*
N -> Tama�o de la matriz A (NxN)
A -> Matriz
n -> Cantidad de enteros contenidos en numeros
numeros -> Lista de n�meros de los que se buscar�n las distancias
distancias -> Resultado con las distancias (un resultado por n�mero)
entorno -> Entorno OpenCL
*/
void ocl(int N,int *A,int n,int *numeros,int *distancias, EntornoOCL_t entorno) {

}
// **************************************************************************
// *************************** FIN IMPLEMENTACI�N ***************************
// **************************************************************************

/*
Recibir� los siguientes par�metros (los par�metros entre corchetes son opcionales): fichEntrada [-d] [-wi work_items] [-wi_wg workitems_por_workgroup]
fichEntrada -> Obligatorio. Fichero de entrada con los par�metros de lanzamiento de los experimentos
-d -> Opcional. Si se indica, se mostrar�n por pantalla los valores iniciales, finales y tiempo de cada experimento
-wi work_items -> Opcional. Si se indica, se lanzar�n tantos work items como se indique en work_items (para OpenCL)
-wi_wg workitems_por_workgroup -> Opcional. Si se indica, se lanzar�n tantos work items en cada work group como se indique en WorkItems_por_WorkGroup (para OpenCL)
*/
int main(int argc,char *argv[]) {
	int i,j,
		debug=0,				   // Indica si se desean mostrar los tiempos y resultados parciales de los experimentos
		num_workitems=0, 		   // N�mero de work items que se utilizar�n
		workitems_por_workgroups=0, // N�mero de work items por cada work group que se utilizar�n
		N,					   // Tama�o de la matriz A (NxN)
		cuantos,				   // N�mero de experimentos
		n,					   // N�mero de enteros a leer del fichero
		semilla,				   // Semilla para la generaci�n de n�meros aleatorios
		maximo,				   // Valor m�ximo de para la generaci�n de valores aleatorios de la matriz A (se generan entre 0 y maximo-1)
		*A,					   // Matriz A. Se representa en forma de vector. Para acceder a la fila f y la columna c: A[f*N+c]
		*numeros,				   // Vector que contiene los n�meros de los que se calcular�n las distancias
		*distancias;			   // Vector resultado conteniendo las distancias calculadas
	long long ti,				   // Tiempo inicial
			tf,				   // Tiempo final
			tt=0; 			   // Tiempo acumulado de los tiempos parciales de todos los experimentos realizados
	FILE *f;					   // Fichero con los datos de entrada
	EntornoOCL_t entorno; 		   //Entorno para el control de OpenCL

	if (!ObtenerParametros(argc, argv, &debug, &num_workitems, &workitems_por_workgroups)) {
		printf("Ejecuci�n incorrecta\nEl formato correcto es %s fichEntrada [-d] [-wi work_items] [-wi_wg workitems_por_workgroup]\n", argv[0]);
		return 0;
	}
	
	InicializarEntornoOCL(&entorno);

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
		ocl(N,A,n,numeros,distancias,entorno);
		tf=mseconds(); 
		tt+=tf-ti;

		if (debug)	{
			printf("Tiempo del experimento %d: %Ld ms\n", i, tf-ti);
			printf("Resultado del experimento %d:\n", i); escribirresult(distancias,n);
		}

		free(A);
		free(numeros);
		free(distancias);
	}

	LiberarEntornoOCL(&entorno);
	printf("Tiempo total de %d experimentos: %Ld ms\n", cuantos, tt);
	fclose(f);

  return 0;
}
