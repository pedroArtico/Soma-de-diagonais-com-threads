#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>


#include "thread.h"

#include "../util/util.h"
#include "../datadefine.h"
#include "../dataStructures/array/arrayMngr.h"
#include "../dataStructures/matrix/matrixMngr.h"

/*==================================================================================
								THREAD - SOLUÇÃO A
===================================================================================*/
void * threadSumFunc_A(void * args){
	ThreadArgsInfo_A * tinfo = (ThreadArgsInfo_A*) args;//Convertendo argumentos para argumentos de thread
	unsigned short int diagProcess;//guardar número da diagonal atual
	float sum = 0, rsp = 0;//soma, auxiliar
	
	//PEGAR UMA DIAGONAL PARA PROCESSAR
	while(1){	
		Coords crds;//coordenadas
		pthread_mutex_lock(tinfo->lock);//down no mutex /*Acessando a região crítica*/
			diagProcess = (*(tinfo->contDiag))++;//pegando o número da diagonal a ser processada
			//*(tinfo->contDiag) ++;//incrementando a diagonal
		pthread_mutex_unlock(tinfo->lock);//up no mutex
		
		if(diagProcess >= tinfo->mx->diagNum)//Se a diagonal lida não pertencer à matriz
			break;//para terminar a thread
		
		//PEGAR PRIMEIRO ELEMENTO PARA INICIAR PROCESSAMENTO
		diagNumToCoord(*(tinfo->mx), diagProcess, &crds);
		getElement(*(tinfo->mx), crds, &sum);
		tinfo->rspArr->data[diagProcess].rspi.countDiagElem = 1;
		//sum = rsp;//definir soma como primeiro
		
		//PERCORRENDO ELEMENTOS DA DIAGONAL
		while(getNextElementPositionMdiags(*(tinfo->mx), &crds)){//enquanto existir um próximo elemento na diagonal
			getElement(*(tinfo->mx), crds, &rsp);//acessando elemento
			sum += rsp;//adicionando elemento na soma
			tinfo->rspArr->data[diagProcess].rspi.countDiagElem++;
		}
		tinfo->rspArr->data[diagProcess].rspi.rsp = sum;//guardando soma da diagonal processada
		tinfo->rspArr->data[diagProcess].rspi.tid = pthread_self();//gravando resultado na matriz de resultado
	}		
	pthread_exit(NULL);//terminando a thread
    return NULL;
}

/*==================================================================================
								THREAD - SOLUÇÃO B
===================================================================================*/
void * threadSumFunc_B(void * args){
    ThreadArgsInfo_B *targs = (ThreadArgsInfo_B*)args;//Convertendo os argumentos da thread
    unsigned int jmp;//Tamanho do pulo
    Coords coordrsp;
    float sum, rsp;

    for(jmp = targs->threadNum; jmp <= (*(targs->mx)).diagNum; (jmp += targs->totThreads)){//Cada thread sempre processará diagonais alternadas com o mesmo tamanho
		if(!diagNumToCoord(*(targs->mx), jmp, &coordrsp)){//convertendo número da diagonal para o primeiro elemento
			continue;
		}
		getElement(*(targs->mx), coordrsp, &sum);
		targs->rspArr->data[jmp].rspi.countDiagElem = 1;
		
        while(getNextElementPositionMdiags(*(targs->mx), &coordrsp)){
			getElement(*(targs->mx), coordrsp, &rsp);
			sum += rsp;
			targs->rspArr->data[jmp].rspi.countDiagElem++;
		}   
        targs->rspArr->data[jmp].rspi.rsp = sum;//gravando resultado na matriz de resultado
        targs->rspArr->data[jmp].rspi.tid = pthread_self();//gravando resultado na matriz de resultado
    }
    pthread_exit(NULL);//terminando a thread
    return NULL;
}


