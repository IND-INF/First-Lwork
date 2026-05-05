#include <stdio.h>
#include <stdlib.h>
#include "scalar_algebra.h"

typedef struct _struct_RealNumber{
    double value;
} RealNumber;

void addRealNumber(const void *arg1, const void *arg2, void *result){

    RealNumber *res = (RealNumber*)result;
    const RealNumber *a1 = (const RealNumber*)arg1;
    const RealNumber *a2 = (const RealNumber*)arg2;

    res->value = a1->value + a2->value;

}

void multiplyRealNumber(const void *arg1, const void *arg2, void *result){

    RealNumber *res = (RealNumber*)result;
    const RealNumber *a1 = (const RealNumber*)arg1;
    const RealNumber *a2 = (const RealNumber*)arg2;

    res->value = a1->value * a2->value;

}

void printRealNumber(const void *element){
    const RealNumber *number = (const RealNumber*)element;
    printf("%lf", number->value);
}

int scanRealNumber(void *element){
    RealNumber *number = (RealNumber*)element;
    return scanf("%lf", &(number->value));
}

int sScanRealNumber(const char *input, void *element){
    RealNumber *number = (RealNumber*)element;
    return sscanf(input, "%lf", &(number->value));
}

int sNScanRealNumber(const char *input, void *element, int *stopIndex){
    RealNumber *number = (RealNumber*)element;
    return sscanf(input, "%lf%n", &(number->value), stopIndex);
}

static Algebra *reAlgebra = NULL;
const Algebra *getRealNumberAlgebra(void){

    if(!reAlgebra){

        if(!(reAlgebra = malloc(sizeof(*reAlgebra)))){
            return reAlgebra;
        }

        reAlgebra->add = addRealNumber;
        reAlgebra->multiply = multiplyRealNumber;

        reAlgebra->print = printRealNumber;
        reAlgebra->scan = scanRealNumber;
        reAlgebra->sScan = sScanRealNumber;
        reAlgebra->sNScan = sNScanRealNumber;

        reAlgebra->elementSize = sizeof(RealNumber);

    }

    return reAlgebra;

}

void setRealNumber(void *element, double value){
    RealNumber *number = (RealNumber*)element;
    number->value = value;
}

void getRealNumber(const void *element, double *value){
    const RealNumber *number = (const RealNumber*)element;
    *value = number->value;
}