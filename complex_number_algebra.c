#include <stdio.h>
#include <stdlib.h>
#include "scalar_algebra.h"

typedef struct ComplexNumber{
    double reValue;
    double imValue;
} ComplexNumber;

void addCompl(const void *arg1, const void *arg2, void *result){
    ComplexNumber *res = (ComplexNumber*)result;
    const ComplexNumber *a1 = (const ComplexNumber*)arg1;
    const ComplexNumber *a2 = (const ComplexNumber*)arg2;
    res->reValue = a1->reValue + a2->reValue;
    res->imValue = a1->imValue + a2->imValue;
}

void multiplyCompl(const void *arg1, const void *arg2, void *result){
    ComplexNumber *res = (ComplexNumber*)result;
    const ComplexNumber *a1 = (const ComplexNumber*)arg1;
    const ComplexNumber *a2 = (const ComplexNumber*)arg2;
    res->reValue = a1->reValue * a2->reValue - a1->imValue * a2->imValue;
    res->imValue = a1->reValue * a2->imValue + a1->imValue * a2->reValue;
}

void printComplexNumber(const void *element){
    printf("%lf+%lfi", ((ComplexNumber*)element)->reValue, ((ComplexNumber*)element)->imValue);
}

Algebra *complAlgebra = NULL;
const Algebra *getComplexNumberAlgebra(void){
    if(!complAlgebra){
        if(!(complAlgebra = malloc(sizeof(*complAlgebra)))){
            return complAlgebra;
        }
        complAlgebra->add = addCompl;
        complAlgebra->multiply = multiplyCompl;
        complAlgebra->print = printComplexNumber;
        complAlgebra->elementSize = sizeof(ComplexNumber);
    }
    return complAlgebra;
}

void assignComplexNumber(void *element, double realPartValue, double imaginaryPartValue){
    ((ComplexNumber*)element)->reValue = realPartValue;
    ((ComplexNumber*)element)->imValue = imaginaryPartValue;
}

void recieveComplexNumber(void *element, double *realPartValue, double *imaginaryPartValue){
    *realPartValue = ((ComplexNumber*)element)->reValue;
    *imaginaryPartValue = ((ComplexNumber*)element)->imValue;
}