#ifndef FILEMNGR_H
#define FILEMNGR_H

#include <stdio.h>
#include <stdlib.h>
#include "fileMngr.h"
#include "../datadefine.h"
#include "../dataStructures/matrix/matrixMngr.h"

bool fileToMatrix(MatrixDescriber mx, char * filename);
void arrayFloatToFile(ArrayDescriber arr, char * filename);
void fillFileWithValue(char * filename, int qtd, float value);
void arrayFloatToFileWithBuffer(ArrayDescriber arr, char * filename);
void generateRandomFloatFile(char * filename, int qtd);

#endif //PROJSO_FILEMNGR_H
