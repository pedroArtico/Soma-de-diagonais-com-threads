#include <stdlib.h>
#include <stdbool.h>
#include "../../datadefine.h"


//Função para alocar um vetor na memória
bool createArray(ArrayDescriber *arr, unsigned int size){
    arr->bottom = -1;
    arr->size = size;
    arr->top = -1;

    arr->data = malloc(size * sizeof(UData));

    if(arr->data == NULL){
        return false;
    }
    return true;
}

//Função para liberar um array da memória
void deleteArray(ArrayDescriber *arr){
	free(arr->data);
	arr->data = NULL;
}

//Função para limpar o conteúdo do array setando todas as variáveis para 0 ou NULL
void cleanArray(ArrayDescriber arr){
	register int i;

	UData cleanData;
	cleanData.value = 0;
	cleanData.rspi.rsp = 0;
	cleanData.rspi.countDiagElem = 0;
	
	
	for(i = 0; i < arr.size; i++){
		arr.data[i] = cleanData;
	}
}
