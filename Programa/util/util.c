#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../datadefine.h"
#include "../file/fileMngr.h"
#include "../dataStructures/array/arrayMngr.h"
#include "../dataStructures/matrix/matrixMngr.h"
#include <pthread.h>


//Função que calcula a diferença de tempo entre dois horários
double timediff(clock_t t1, clock_t t2){
	if(t1 > t2){
		return((double)(t1 - t2)) / (CLOCKS_PER_SEC * 1000);
	}else{
		return((double)(t2 - t1)) / (CLOCKS_PER_SEC * 1000);
	}
}

//Imprime uma mensagem de pausa na tela 
void pauseMsg(){
	printf("\nPressione enter para continuar...\n");
	getchar();
}

void putDefaultTitle(char * title, int h){
	char ch;
	if(h < 3)
		ch = '-';
	else
		ch = '#';
	int i;
	puts("\n");
	for(i = 0; i < h*10; i++){
		putchar(ch);
	}
	printf(" [  %s  ]  ", title);
	for(i = 0; i < h*10; i++){
		putchar(ch);
	}
	printf("\n\n");
}

//Gerencia a entrada de dados do usuário e informa quando saída foi requerida
bool inputFromUser(Input *rsp){

	putDefaultTitle("ENTRADA DE DADOS", 3);
	printf("Numero de threads:  ");
	fscanf(stdin,"%d", &rsp->numThreads);
	printf("Matriz (m linhas):  ");
	fscanf(stdin, "\n%d", &rsp->matrixM);
	printf("Matriz (n colunas): ");
	fscanf(stdin, "\n%d", &rsp->matrixN);
	getchar();
	
	putDefaultTitle("Voce escolheu", 1);
	
	printf("Processar uma matriz: \t[%d x %d]\n", rsp->matrixM, rsp->matrixN);
	printf("Usando:\t\t\t%d threads\n", rsp->numThreads);

	
	pauseMsg();

	if(rsp->numThreads <= 0 || rsp->matrixM <= 0 || rsp->matrixN <= 0){
		return false;
	}
	return true;
	
}

//Auxiliar para tabela
void tblHSep(){
	printf("\t+---------------+---------------+------------------+------------------+\n");
}

//Imprime uma tabela com os resultados da soma
void OutputSumToUser(MatrixDescriber *matrix, ArrayDescriber *rspArr){
	register int i;
	char vertSepChar = '|';
	
	putDefaultTitle("IMPRIMINDO RESULTADOS", 2);
	tblHSep();
	printf("\t%c COORDENADAS DO%c NUMERO DA\t%c  RESULTADO DA\t   %c  SOMAS FEITAS    %c\n",vertSepChar,vertSepChar,vertSepChar,vertSepChar,vertSepChar);
    printf("\t%c PRIMEIRO ELEM.%c DIAGONAL \t%c       SOMA\t   %c   NA DIAGONAL    %c\n",vertSepChar,vertSepChar,vertSepChar,vertSepChar,vertSepChar);
	tblHSep();
	for(i = 0; i < matrix->diagNum; i++){
    	Coords c;
    	diagNumToCoord(*matrix, i, &c);
        printf("\t%c  (%4d,%4d)  %c     %5d     %c %10.5g  \t   %c %9d        %c",vertSepChar,c.mpos, c.npos,vertSepChar, i,vertSepChar, rspArr->data[i].rspi.rsp, vertSepChar, rspArr->data[i].rspi.countDiagElem, vertSepChar);
	
		puts("");
	}					
	tblHSep();
}

//Função que gera um gráfico com a execução feita por cada thread
void printThreadWork(ArrayDescriber rspArr, ArrayDescriber tidArr, MatrixDescriber mx, unsigned int numt){
	register int i, j, totSum;
	
	//Vetor auxiliar
	ArrayDescriber twork;
	createArray(&twork, numt);
	
	//Vetor auxiliar
	ArrayDescriber tdiags;
	createArray(&tdiags, numt);
	
	//Limpando vetores auxiliares já que serão usados como contadores
	cleanArray(twork);
	cleanArray(tdiags);
	
	totSum = mx.m * mx.n;
	
	for(i = 0; i < mx.diagNum; i++){//Percorrer vetor de resposta
		for(j = 0; j < numt; j++){//Percorrer vetor de threads
			if(pthread_equal(rspArr.data[i].rspi.tid, tidArr.data[j].tid)){
				twork.data[j].value += rspArr.data[i].rspi.countDiagElem;  //Soma da quantidade de somas
				tdiags.data[j].value ++;//Soma das diagonais processadas
			}
		}
	}
	
	for(i = 0; i < numt; i++){
		printf("THREAD [%3d]|", i);
		for(j = 0; j < (twork.data[i].value *80 / totSum); j++){
			putchar('*');
		}
		printf("   [ %d somas ]",twork.data[i].value);
		printf(" [ %d diagonais ]\n",tdiags.data[i].value);
	}
	
	deleteArray(&twork);
	deleteArray(&tdiags);
}


