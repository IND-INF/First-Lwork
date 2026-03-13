#include <stdio.h>
#include <stdlib.h>
#include "scalar_algebra.h"

typedef struct Integer{
    int value;
} Integer;

void addInt(const void *arg1, const void *arg2, void *result){
    Integer *res = (Integer*)result;
    const Integer *a1 = (const Integer*)arg1;
    const Integer *a2 = (const Integer*)arg2;
    res->value = a1->value + a2->value;
}

void multiplyInt(const void *arg1, const void *arg2, void *result){
    Integer *res = (Integer*)result;
    const Integer *a1 = (const Integer*)arg1;
    const Integer *a2 = (const Integer*)arg2;
    res->value = a1->value * a2->value;
}

void printInteger(const void *element){
    printf("%d", ((Integer*)element)->value);
}

Algebra *intAlgebra = NULL;
const Algebra *getIntegerAlgebra(void){
    if(!intAlgebra){
        if(!(intAlgebra = malloc(sizeof(*intAlgebra)))){
            return intAlgebra;
        }
        intAlgebra->add = addInt;
        intAlgebra->multiply = multiplyInt;
        intAlgebra->print = printInteger;
        intAlgebra->elementSize = sizeof(Integer);
    }
    return intAlgebra;
}

void assignInteger(void *element, int value){
    ((Integer*)element)->value = value;
}

void recieveInteger(void *element, int *value){
    *value = ((Integer*)element)->value;
}