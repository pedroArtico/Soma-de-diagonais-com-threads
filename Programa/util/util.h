#ifndef UTIL_H
#define UTIL_H
	#include "../datadefine.h"

	//Protótipos
	double timediff(clock_t t1, clock_t t2);//Calcula tempo entre dois intervalos
	void pauseMsg();//Mostra uma mensagem de pausa
	bool inputFromUser(Input *in);
	void OutputSumToUser(MatrixDescriber *matrix, ArrayDescriber *rspArr);
	void putDefaultTitle(char * title, int h);
	void printThreadWork(ArrayDescriber rspArr, ArrayDescriber tidArr, MatrixDescriber mx, unsigned int numt);
#endif