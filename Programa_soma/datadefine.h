#ifndef DATADEFINE_H
#define DATADEFINE_H
#include <pthread.h>
#include <stdbool.h>
												
/*============================
	TIPO GERAL DE DADO
=============================*/
typedef struct{
	float rsp;
	pthread_t tid;
	int countDiagElem;
}ResultAndThreadInfo;

typedef
    union{
        pthread_t tid;
        //float rsp;
        int value;
        ResultAndThreadInfo rspi; 
}UData;



/*============================
	MATRIZ
=============================*/
//Descritor de matriz
typedef struct mxdccb{
    float ** data;
    int m, n, diagNum;
}MatrixDescriber;


/*============================
	VETOR
=============================*/
//Descritor de vetor
typedef struct{
	UData * data;
    int top, bottom;
    unsigned int size;
}ArrayDescriber;

/*============================
	THREAD
=============================*/
//Argumentos para thread de soma (solução A)
typedef struct{
    MatrixDescriber * mx;
    ArrayDescriber *rspArr;
    pthread_mutex_t *lock;
    unsigned short int *contDiag;
}ThreadArgsInfo_A;

//Argumentos para thread de soma (solução B)
typedef struct{
    MatrixDescriber * mx;
    unsigned short int threadNum;
    unsigned int totThreads;
    ArrayDescriber *rspArr;
}ThreadArgsInfo_B;

/*============================
	TIPOS DE DADO AUXILIARES
=============================*/
//Informações sobre a execução do programa
typedef struct 
{
	double elapsedTime;
	int m, n, diags, numThreads;
}ExecutionData;

//Tipo de dado para guardar coordenadas de uma matriz
typedef struct {
    unsigned short int mpos, npos;
}Coords;

//Entrada para processamento
typedef struct 
{
	int numThreads, matrixM, matrixN;
	char *inputPath, *outputPath;
}Input;



#endif //PROJSO_DATADEFINE_H
