#include <stdio.h>
#include <stdlib.h>
#include "scalar_algebra.h"

typedef struct _struct_ComplexNumber{
    double reValue;
    double imValue;
} ComplexNumber;

void addComplexNumber(const void *arg1, const void *arg2, void *result){

    ComplexNumber *res = (ComplexNumber*)result;
    const ComplexNumber *a1 = (const ComplexNumber*)arg1;
    const ComplexNumber *a2 = (const ComplexNumber*)arg2;

    res->reValue = a1->reValue + a2->reValue;
    res->imValue = a1->imValue + a2->imValue;

}

void multiplyComplexNumber(const void *arg1, const void *arg2, void *result){

    ComplexNumber *res = (ComplexNumber*)result;
    const ComplexNumber *a1 = (const ComplexNumber*)arg1;
    const ComplexNumber *a2 = (const ComplexNumber*)arg2;

    res->reValue = a1->reValue * a2->reValue - a1->imValue * a2->imValue;
    res->imValue = a1->reValue * a2->imValue + a1->imValue * a2->reValue;

}

void printComplexNumber(const void *element){
    const ComplexNumber *number = (const ComplexNumber*)element;
    printf("%lf+%lfi", number->reValue, number->imValue);
}

int scanComplexNumber(void *element){
    ComplexNumber *number = (ComplexNumber*)element;
    return scanf("%lf+%lfi", &(number->reValue), &(number->imValue));
}

int sScanComplexNumber(const char *input, void *element){
    ComplexNumber *number = (ComplexNumber*)element;
    return sscanf(input, "%lf+%lfi", &(number->reValue), &(number->imValue));
}

int sNScanComplexNumber(const char *input, void *element, int *stopIndex){
    ComplexNumber *number = (ComplexNumber*)element;
    return sscanf(input, "%lf+%lfi%n", &(number->reValue), &(number->imValue), stopIndex);
}

static Algebra *complAlgebra = NULL;
const Algebra *getComplexNumberAlgebra(void){

    if(!complAlgebra){
        
        if(!(complAlgebra = malloc(sizeof(*complAlgebra)))){
            return complAlgebra;
        }

        complAlgebra->add = addComplexNumber;
        complAlgebra->multiply = multiplyComplexNumber;

        complAlgebra->print = printComplexNumber;
        complAlgebra->scan = scanComplexNumber;
        complAlgebra->sScan = sScanComplexNumber;
        complAlgebra->sNScan = sNScanComplexNumber;

        complAlgebra->elementSize = sizeof(ComplexNumber);

    }

    return complAlgebra;
    
}

void setComplexNumber(void *element, double realPartValue, double imaginaryPartValue){
    ComplexNumber *number = (ComplexNumber*)element;
    number->reValue = realPartValue;
    number->imValue = imaginaryPartValue;
}

void getComplexNumber(const void *element, double *realPartValue, double *imaginaryPartValue){
    const ComplexNumber *number = (const ComplexNumber*)element;
    *realPartValue = number->reValue;
    *imaginaryPartValue = number->imValue;
}