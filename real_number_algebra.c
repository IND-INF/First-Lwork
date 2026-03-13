#include <stdio.h>
#include <stdlib.h>
#include "scalar_algebra.h"

typedef struct RealNumber{
    double value;
} RealNumber;

void addRe(const void *arg1, const void *arg2, void *result){
    RealNumber *res = (RealNumber*)result;
    const RealNumber *a1 = (const RealNumber*)arg1;
    const RealNumber *a2 = (const RealNumber*)arg2;
    res->value = a1->value + a2->value;
}

void multiplyRe(const void *arg1, const void *arg2, void *result){
    RealNumber *res = (RealNumber*)result;
    const RealNumber *a1 = (const RealNumber*)arg1;
    const RealNumber *a2 = (const RealNumber*)arg2;
    res->value = a1->value * a2->value;
}

void printRealNumber(const void *element){
    printf("%lf", ((RealNumber*)element)->value);
}

Algebra *reAlgebra = NULL;
const Algebra *getRealNumberAlgebra(void){
    if(!reAlgebra){
        if(!(reAlgebra = malloc(sizeof(*reAlgebra)))){
            return reAlgebra;
        }
        reAlgebra->add = addRe;
        reAlgebra->multiply = multiplyRe;
        reAlgebra->print = printRealNumber;
        reAlgebra->elementSize = sizeof(RealNumber);
    }
    return reAlgebra;
}

void assignRealNumber(void *element, double value){
    ((RealNumber*)element)->value = value;
}

void recieveRealNumber(void *element, double *value){
    *value = ((RealNumber*)element)->value;
}