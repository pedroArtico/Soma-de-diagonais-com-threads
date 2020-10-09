
//Incluindo bibliotecas padr�o
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <strings.h>
#include <unistd.h>

//Incluindo bibliotecas criadas
#include "util/util.h"							//Biblioteca com fun��es de utilidades diversas
#include "datadefine.h"							//Defini��o de dados (vari�veis globais, defines e tipos de dados novos)
#include "dataStructures/array/arrayMngr.h"		//Biblioteca para gerenciar o tipo de vetor usado
#include "dataStructures/matrix/matrixMngr.h"	//Biblioteca para gerenciar o tipo de matriz usada
#include "thread/thread.h"						//Biblioteca contendo fun��es executadas por threads
#include "file/fileMngr.h"						//Bilioteca para definir gerenciamento dos arquivos usados


double execB(Input in, bool printResult, bool printMx);
double execA(Input in, bool printResult, bool printMx);


//////////////////// DEFAULT/CONFIGURATION VALUES ///////////////////////////
//Dimens�es padr�o da matriz
#define DEFAULT_INPUT_PATH "in.txt"
#define DEFAULT_OUTPUT_PATH_A "out_sol_A.txt"
#define DEFAULT_OUTPUT_PATH_B "out_sol_B.txt"
///////////////////////////////////////////////////////////////////////////


/*==================================================================================
								MAIN
===================================================================================*/
int main(){	
	Input in;
	in.inputPath = DEFAULT_INPUT_PATH;
	while(inputFromUser(&in)){
		
		in.outputPath = DEFAULT_OUTPUT_PATH_A;
		putDefaultTitle("SOLUCAO A",5);
		execA(in, true, ((in.matrixN <= 20 && in.matrixN < 30) ? true : false));

		in.outputPath = DEFAULT_OUTPUT_PATH_B;
		putDefaultTitle("SOLUCAO B",5);
		execB(in, true, ((in.matrixN <= 20 && in.matrixN < 30) ? true : false));
		printf("\n\n\n\n");
	}
	return 0;
}



/*==================================================================================
								SOLU��O A
===================================================================================*/
double execA(Input in, bool printResult, bool printMx){	
	//DECLARA��O DE VARI�VEIS
	time_t tStart = 0, tEnd = 0;//Declarando rel�gios
	
	tStart = clock();//DISPARANDO REL�GIO
	
	pthread_mutex_t lock;//Declarando mutex
	pthread_mutex_init(&lock, NULL);//Iniciando mutex
	double elapsedTime;//Armazenamento do tempo gasto
	register int i;
	
	unsigned short int contDiag = 0;

	//Definindo vetores e matrizes
    MatrixDescriber matrix;
    ArrayDescriber rspArr;
    ArrayDescriber tidArr;

	//Criando vetores e matrizes
	createMatrix(&matrix, in.matrixM, in.matrixN);
    createArray(&rspArr, matrix.diagNum);
	createArray(&tidArr, in.numThreads);
	rspArr.top = matrix.diagNum-1;
	
	//Lendo arquivo de entrada
    if(!fileToMatrix(matrix, in.inputPath)){
		printf("ATENCAO!\nO arquivo de entrada era muito pequeno,\nposicoes vazias foram preenchidas com: -1\n");
	}
	
	if(printMx){
		printMatrix(matrix);
	}
	
	ThreadArgsInfo_A targs;
	targs.mx = &matrix;
	targs.rspArr = &rspArr;
	targs.lock = &lock;
	targs.contDiag = &contDiag;

	
	for(i = 0; i < in.numThreads; i++){
		pthread_create(&(tidArr.data[i].tid), NULL, threadSumFunc_A, &targs);
	}
	for(i = 0; i < in.numThreads; i++){
		pthread_join(tidArr.data[i].tid, NULL);
	}
	
	pthread_mutex_destroy(&lock);//destruindo o mutex
	
	arrayFloatToFile(rspArr, in.outputPath);//Gravando resultado no arquivo
	
	if(printResult){
		printf("\n");
		OutputSumToUser(&matrix, &rspArr);
		printf("\n");
		printThreadWork(rspArr, tidArr, matrix, in.numThreads);
		printf("\n");
	}

	
 	//Liberando mem�ria utilizada
 	deleteMatrix(&matrix);
 	deleteArray(&rspArr);
	deleteArray(&tidArr);
	
	tEnd = clock();//PARANDO REL�GIO
	elapsedTime = timediff(tStart, tEnd);
	return elapsedTime;
}
/*==================================================================================
								SOLU��O B
===================================================================================*/
double execB(Input in, bool printResult, bool printMx){
	time_t tStart, tEnd;
	
	tStart = clock();//DISPARANDO REL�GIO
	register int i;

	//Definindo vetores e matrizes
    MatrixDescriber matrix;
    ArrayDescriber rspArr;
    ArrayDescriber tidArr;

	//Criando vetores e matrizes
	createMatrix(&matrix, in.matrixM, in.matrixN);
    createArray(&rspArr, matrix.diagNum);
	createArray(&tidArr, in.numThreads);
	rspArr.top = matrix.diagNum-1;
	
	//Lendo arquivo de entrada
    if(!fileToMatrix(matrix, in.inputPath)){
		printf("ATENCAO!\nO arquivo de entrada era muito pequeno,\nposicoes vazias foram preenchidas com algum numero\n");
	}
	
	if(printMx){
		printMatrix(matrix);
	}
	
	//Alocando na mem�ria espa�o para o argumento das threads
	ThreadArgsInfo_B * tinfoptr = (ThreadArgsInfo_B*) malloc(in.numThreads * sizeof(ThreadArgsInfo_B));
	if(tinfoptr == NULL){
		fprintf(stderr,"Erro ao alocar argumentos de threads\n");
		exit(-1);
	}

	//Para cada thread
    for(i = 0; i < in.numThreads; i++){

		//Inicializando os argumentos das threads
		tinfoptr[i].threadNum = (unsigned short int)i;
        tinfoptr[i].mx = &matrix;
    	tinfoptr[i].rspArr = &rspArr;
    	tinfoptr[i].totThreads = (unsigned int)in.numThreads;
        
        //Criando thread
		pthread_create(&(tidArr.data[i].tid), NULL, threadSumFunc_B, &tinfoptr[i]);
    }
    
    for(i = 0; i < in.numThreads; i++){
		pthread_join(tidArr.data[i].tid, NULL);
	}
	
	arrayFloatToFile(rspArr, in.outputPath);
    //arrayFloatToFileWithBuffer(rspArr, in.outputPath);
	
	if(printResult){
		printf("\n");
		OutputSumToUser(&matrix, &rspArr);
		printf("\n");
		printThreadWork(rspArr, tidArr, matrix, in.numThreads);
		printf("\n");
	}
    
    
 	//Liberando mem�ria utilizada
 	deleteMatrix(&matrix);
 	deleteArray(&rspArr);
	deleteArray(&tidArr);
 	free(tinfoptr);

    tEnd = clock();//PARANDO REL�GIO
    return timediff(tStart, tEnd);
}



