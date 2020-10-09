/**
Escrito por André L. R. Estevam 		como trabalho para a disciplina de Sistemas
Operacionais (1º Semestre de 2017) da Faculdade de Tecnologia da Unicamp

#################################################################################
Este é um programa que lê números com ponto flutuante a partir de um arquivo para
uma matriz (m x n) com m e n sendo inseridos pelo usuário, o programa usa de 1 a n
threads para processar a soma das diagonais principais armazenando o resultado em
arquivo e exibindo-o na tela, podendo ainda gerar dados sobre a execução do programa 
gravados em arquivo. 
#################################################################################
*/

//Incluindo bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <strings.h>
#include <unistd.h>

//Incluindo bibliotecas criadas
#include "util/util.h"							//Biblioteca com funções de utilidades diversas
#include "datadefine.h"							//Definição de dados (variáveis globais, defines e tipos de dados novos)
#include "dataStructures/array/arrayMngr.h"		//Biblioteca para gerenciar o tipo de vetor usado
#include "dataStructures/matrix/matrixMngr.h"	//Biblioteca para gerenciar o tipo de matriz usada
#include "thread/thread.h"						//Biblioteca contendo funções executadas por threads
#include "file/fileMngr.h"						//Bilioteca para definir gerenciamento dos arquivos usados


double execB(Input in, bool printResult, bool printMx);
double execA(Input in, bool printResult, bool printMx);


//////////////////// DEFAULT/CONFIGURATION VALUES ///////////////////////////
//Dimensões padrão da matriz
static const int default_M 					= 1000; 
static const int default_N 					= 1000;

//SOLUÇÃO UTILIZADO <a: SOLUÇÃO a|b: SOLUÇÃO b|outro: se generateDataMode ativado, ambos>
const char defaultMethod 					= 'c';

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
								SOLUÇÃO A
===================================================================================*/
double execA(Input in, bool printResult, bool printMx){	
	//DECLARAÇÃO DE VARIÁVEIS
	time_t tStart = 0, tEnd = 0;//Declarando relógios
	
	tStart = clock();//DISPARANDO RELÓGIO
	
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

	
 	//Liberando memória utilizada
 	deleteMatrix(&matrix);
 	deleteArray(&rspArr);
	deleteArray(&tidArr);
	
	tEnd = clock();//PARANDO RELÓGIO
	elapsedTime = timediff(tStart, tEnd);
	return elapsedTime;
}
/*==================================================================================
								SOLUÇÃO B
===================================================================================*/
double execB(Input in, bool printResult, bool printMx){
	time_t tStart, tEnd;
	
	tStart = clock();//DISPARANDO RELÓGIO
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
		printf("ATENCAO!\nO arquivo de entrada era muito pequeno,\n posicoes vazias foram preenchidas com algum numero\n");
	}
	
	if(printMx){
		printMatrix(matrix);
	}
	
	//Alocando na memória espaço para o argumento das threads
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
    
    
 	//Liberando memória utilizada
 	deleteMatrix(&matrix);
 	deleteArray(&rspArr);
	deleteArray(&tidArr);
 	free(tinfoptr);

    tEnd = clock();//PARANDO RELÓGIO
    return timediff(tStart, tEnd);
}



