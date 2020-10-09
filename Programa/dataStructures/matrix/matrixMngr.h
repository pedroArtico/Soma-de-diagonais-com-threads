#ifndef MATRIXMNGR_H
#define MATRIXMNGR_H

#include <stdbool.h>
#include "../../datadefine.h"

//Protótipos de funções para gerenciar matrizes
bool createMatrix(MatrixDescriber *matrix, unsigned int m, unsigned int n);
bool getNextElementPositionMdiags(MatrixDescriber mxd, Coords * excCoord);
void getElement(MatrixDescriber mxd, Coords coord, float *rsp);
bool diagNumToCoord(MatrixDescriber mxd, unsigned int coordNum, Coords *rsp);
void fillMatrix(MatrixDescriber mx, float fillValue);
void fillMatrixWithRandom(MatrixDescriber mx);
bool setElement(MatrixDescriber mx, Coords pos, float value);
void deleteMatrix(MatrixDescriber *mx);
void printMatrix(MatrixDescriber mx);

#endif
