#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../datadefine.h"
#include "../../util/util.h"


//Função para criar uma matriz a partir dos valores das m linhas e n colunas
bool createMatrix(MatrixDescriber *matrix, unsigned int m, unsigned int n){

    matrix->diagNum = (m + n) -1;
    matrix->m = m;
    matrix->n = n;
    
    matrix->data = malloc(m * sizeof(float*));//Alocando vetor para vetor de float
    
    if(matrix->data == NULL){
		fprintf(stderr, "Erro ao alocar vetor inicial da matriz\n");
		exit(-1);
		getchar();
		return false;
	}

    register int i;
    for(i = 0; i < m; i++){
        matrix->data[i] = malloc(n * sizeof(float));//Alocando vetor
        if(matrix->data[i] == NULL){
			fprintf(stderr, "Erro ao alocar coluna %d da matriz\n", i);
			exit(-2);
			
			getchar();
			return false;
		}
    }

    return true;
}

//Função para verificar se uma coordenada existe dentro de uma matriz
bool coordIsInsideMatrix(MatrixDescriber mx, Coords pos){
    if(pos.mpos >= mx.m || pos.npos >= mx.n){
        return false;
    }
    return true;
}

//Função que dada uma matriz e uma coordenada retorna a coordenada do próximo elemento no sentido da diagonal principal
bool getNextElementPositionMdiags(MatrixDescriber mxd, Coords * excCoord){
    excCoord->npos++;
    excCoord->mpos++;

    if(!coordIsInsideMatrix(mxd, *excCoord)){
        return false;
    }
    return true;
}

//Função que retorna um elemento da matriz dadas suas coordenadas
void getElement(MatrixDescriber mxd, Coords coord, float *rsp){
    *(rsp) = mxd.data[coord.mpos][coord.npos];
}

//Função que converte o número de uma diagonal para suas coordenadas
bool diagNumToCoord(MatrixDescriber mxd, unsigned int coordNum, Coords *rsp){
    if(coordNum > mxd.diagNum-1 || coordNum < 0){
        return false;
    }
    if(coordNum <= mxd.m - 1){//Diagonais que começam na primeira linha
        rsp->mpos = (mxd.m - 1) - coordNum;
        rsp->npos = 0;
    }
    else{//Diagonais que começam na primeira coluna
        rsp->mpos = 0;
        rsp->npos = coordNum - (mxd.m - 1);
    }
    return true;
}


//Função que preenche toda a matriz com um valor (apenas para testes)
void fillMatrix(MatrixDescriber mx, float fillValue){
    register int i, j;
    for(i = 0; i < mx.m; i++){
        for(j = 0; j < mx.n; j++){
            mx.data[i][j] = fillValue;
        }
    }
}
//Função que preenche toda a matriz com valores aleatórios (apenas para testes)
void fillMatrixWithRandom(MatrixDescriber mx){
	register int i, j;
    for(i = 0; i < mx.m; i++){
        for(j = 0; j < mx.n; j++){
            mx.data[i][j] = rand();
        }
    }
}

//Função que grava numa matriz um valor a partir de suas coordenadas
bool setElement(MatrixDescriber mx, Coords pos, float value){
    if(!coordIsInsideMatrix(mx, pos)){//se coordenada estiver fora da matriz
        return false;
    }
    mx.data[pos.mpos][pos.npos] = value;
    return true;
}

//Função que remove uma matriz da memória
void deleteMatrix(MatrixDescriber *mx){
    register int i;
    for(i = 0; i < mx->m; i++){
        free(mx->data[i]);
        //mx->data[i] = NULL;
    }
    free(mx->data);
    mx->data = NULL;
}

//Função que imprime os valores guardados em uma matriz
void printMatrix(MatrixDescriber mx){
    register int i = 0, j = 0;
    
    putDefaultTitle("MATRIZ", 2);
    for(i = 0; i < mx.n; i++){
		   printf("%5d ", i);	
	}
	printf("\n");
    
    for(i = 0; i < mx.m; i++){
		for(j = 0; j < mx.n; j++){
        	if(j == 0){
				printf("%2d ",i);
			}
			
            printf("[%4.3g]",mx.data[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}
