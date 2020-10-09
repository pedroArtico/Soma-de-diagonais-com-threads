#ifndef ARRAYMNGR_H_
#define ARRAYMNGR_H_

#include <stdbool.h>

//Protótipos
bool createArray(ArrayDescriber *arr, unsigned int size);
void deleteArray(ArrayDescriber *arr);
void cleanArray(ArrayDescriber arr);


#endif 