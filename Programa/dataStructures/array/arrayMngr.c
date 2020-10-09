#include <stdlib.h>
#include <stdbool.h>
#include "../../datadefine.h"


//Fun��o para alocar um vetor na mem�ria
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

//Fun��o para liberar um array da mem�ria
void deleteArray(ArrayDescriber *arr){
	free(arr->data);
	arr->data = NULL;
}

//Fun��o para limpar o conte�do do array setando todas as vari�veis para 0 ou NULL
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
