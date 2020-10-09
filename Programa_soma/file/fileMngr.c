#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileMngr.h"
#include "../datadefine.h"
#include "../dataStructures/matrix/matrixMngr.h"


bool fileToMatrix(MatrixDescriber mx,char * filename) {
	FILE * in = fopen(filename, "r");
	float value;
	register int i, j;
	bool rsp = true;

	if(in == NULL) {
		fprintf(stderr, "Erro ao abrir arquivo para leitura de dados\n");
		exit(-1);
		//return false;
	}
	for(i = 0; i < mx.m; i++) {
		for(j = 0; j < mx.n; j++) {
			if(!feof(in)) {//Enquanto o arquivo n�o tiver terminado de ser lido
				fscanf(in, "%f ", &value);//Lendo registro
			} else {
				rsp = false;//Se arquivo foi lido totalmente mas a matriz n�o foi preenchida totalmente marca que um valor false dever ser retornado
				value = -1.0;//seta posi��es da matriz sem valor correspondente no arquivo para -1 (facilitar localiza��o)
			}
			Coords crd;
			crd.mpos = i;
			crd.npos = j;

			setElement(mx, crd, value);
		}
	}
	fclose(in);
	return rsp;
}


void arrayFloatToFile(ArrayDescriber arr, char * filename) {
	FILE * out = fopen(filename, "w+");

	if(out == NULL) {
		fprintf(stderr, "Erro ao abrir arquivo para escrita de dados\n");
		exit(-1);
		//return;
	}

	int register i;
	for(i = 0; i < arr.top+1; i++) {
		fprintf(out, "%f ", arr.data[i].rspi.rsp);
	}

	fclose(out);
}


void fillFileWithValue(char * filename, int qtd, float value) {
	register int i;
	FILE * fl = fopen(filename, "w");//abrindo arquivo
	if(fl == NULL) {
		return;
	}
	char * buffer = (char*)malloc(10 * sizeof(char) * sizeof(int));//alocando espa�o do buffer
	
	snprintf(buffer, 10 * sizeof(char) * sizeof(float), "%.3f ", value);//gravando no buffer
	
	for(i = 0; i < qtd; i++) {//Gravando repetidamente uma string no arquivo
		fprintf(fl, "%s", buffer);
	}
	
	fclose(fl);
	free(buffer);
}


void generateRandomFloatFile(char * filename, int qtd) {
	FILE * fl = fopen(filename, "w");
	if(fl == NULL) {
		return;
	}
	register int i;
	for(i = 0; i < qtd; i++) {
		fprintf(fl, "%f ",  ((float)(rand() % 60) / 10.0) * 5);
	}
}