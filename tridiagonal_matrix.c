#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scalar_algebra.h"

typedef struct _struct_TriMatrix{
    char *supDia;
    char *princDia;
    char *infDia;
} TriMatrix;

void addTridiagonalMatrices(unsigned int size, const void *matrix1, const void *matrix2,
    void *result, const Algebra *scalarAlgebra){
    TriMatrix *res = (TriMatrix*)result;
    const TriMatrix *m1 = (const TriMatrix*)matrix1;
    const TriMatrix *m2 = (const TriMatrix*)matrix2;
    unsigned int lastMem = (size - 1)*(scalarAlgebra->elementSize);
    for(unsigned int memPlus = 0; memPlus < lastMem; memPlus+=(scalarAlgebra->elementSize)){
        scalarAlgebra->add((m1->supDia)+memPlus, (m2->supDia)+memPlus, (res->supDia)+memPlus);
        scalarAlgebra->add((m1->infDia)+memPlus, (m2->infDia)+memPlus, (res->infDia)+memPlus);
        scalarAlgebra->add((m1->princDia)+memPlus, (m2->princDia)+memPlus, (res->princDia)+memPlus);
    }
    scalarAlgebra->add((m1->princDia)+lastMem, (m2->princDia)+lastMem, (res->princDia)+lastMem);
}

int multiplyTridiagonalMatrices(unsigned int size, const void *matrix1, const void *matrix2,
    void *result, const Algebra *scalarAlgebra){
    TriMatrix *res = (TriMatrix*)result;
    const TriMatrix *m1 = (const TriMatrix*)matrix1;
    const TriMatrix *m2 = (const TriMatrix*)matrix2;
    if(size == 1){
        scalarAlgebra->multiply(m1->princDia, m2->princDia, res->princDia);
        return 1;
    }
    void *subservient1, *subservient2;
    if(!(subservient1 = malloc(scalarAlgebra->elementSize))){
        return 0;
    }
    if(!(subservient2 = malloc(scalarAlgebra->elementSize))){
        free(subservient1);
        return 0;
    }
    unsigned int elSize = scalarAlgebra->elementSize;
    unsigned int lastMem = (size - 1)*elSize;
    unsigned int preLastMem = lastMem - elSize;
    scalarAlgebra->multiply(m1->princDia, m2->princDia, subservient1);
    scalarAlgebra->multiply(m1->supDia, m2->infDia, subservient2);
    scalarAlgebra->add(subservient1, subservient2, res->princDia);
    for(unsigned int memPlus = 0; memPlus < preLastMem; memPlus+=elSize){
        scalarAlgebra->multiply((m1->princDia)+memPlus, (m2->supDia)+memPlus, subservient1);
        scalarAlgebra->multiply((m1->supDia)+memPlus, (m2->princDia)+memPlus+elSize, subservient2);
        scalarAlgebra->add(subservient1, subservient2, (res->supDia)+memPlus);

        scalarAlgebra->multiply((m1->infDia)+memPlus, (m2->princDia)+memPlus, subservient1);
        scalarAlgebra->multiply((m1->princDia)+memPlus+elSize, (m2->infDia)+memPlus, subservient2);
        scalarAlgebra->add(subservient1, subservient2, (res->infDia)+memPlus);
        
        scalarAlgebra->multiply((m1->infDia)+memPlus, (m2->supDia)+memPlus, subservient1);
        scalarAlgebra->multiply((m1->princDia)+memPlus+elSize, (m2->princDia)+memPlus+elSize, subservient2);
        scalarAlgebra->add(subservient1, subservient2, subservient1);
        scalarAlgebra->multiply((m1->supDia)+memPlus+elSize, (m2->infDia)+memPlus+elSize, subservient2);
        scalarAlgebra->add(subservient1, subservient2, (res->princDia)+memPlus+elSize);
    }
    scalarAlgebra->multiply((m1->princDia)+preLastMem, (m2->supDia)+preLastMem, subservient1);
    scalarAlgebra->multiply((m1->supDia)+preLastMem, (m2->princDia)+lastMem, subservient2);
    scalarAlgebra->add(subservient1, subservient2, (res->supDia)+preLastMem);

    scalarAlgebra->multiply((m1->infDia)+preLastMem, (m2->princDia)+preLastMem, subservient1);
    scalarAlgebra->multiply((m1->princDia)+lastMem, (m2->infDia)+preLastMem, subservient2);
    scalarAlgebra->add(subservient1, subservient2, (res->infDia)+preLastMem);
    
    scalarAlgebra->multiply((m1->infDia)+preLastMem, (m2->supDia)+preLastMem, subservient1);
    scalarAlgebra->multiply((m1->princDia)+lastMem, (m2->princDia)+lastMem, subservient2);
    scalarAlgebra->add(subservient1, subservient2, (res->princDia)+lastMem);
    free(subservient1); free(subservient2);
    return 1;
}

void scalarlyMultiplyTridiagonalMatrix(unsigned int size, const void *scalar, const void *matrix,
    void *result, const Algebra *scalarAlgebra){
    TriMatrix *res = (TriMatrix*)result;
    const TriMatrix *mat = (const TriMatrix*)matrix;
    unsigned int lastMem = (size - 1)*(scalarAlgebra->elementSize);
    for(unsigned int memPlus = 0; memPlus < lastMem; memPlus+=(scalarAlgebra->elementSize)){
        scalarAlgebra->multiply(scalar, (mat->supDia)+memPlus, (res->supDia)+memPlus);
        scalarAlgebra->multiply(scalar, (mat->infDia)+memPlus, (res->infDia)+memPlus);
        scalarAlgebra->multiply(scalar, (mat->princDia)+memPlus, (res->princDia)+memPlus);
    }
    scalarAlgebra->multiply(scalar, (mat->princDia)+lastMem, (res->princDia)+lastMem);
}

void createTridiagonalMatrix(void **memArray, void *matrix){
    TriMatrix *mat = (TriMatrix*)matrix;
    mat->supDia = (char*)(*memArray);
    mat->princDia = (char*)(*(memArray + 1));
    mat->infDia = (char*)(*(memArray + 2));
}

void assignTridiagonalMatrix(unsigned int size, void *matrix, const void *element, const Algebra *scalarAlgebra){
    TriMatrix *mat = (TriMatrix*)matrix;
    unsigned int elSize = scalarAlgebra->elementSize;
    unsigned int lastMem = (size - 1)*elSize;
    for(unsigned int memPlus = 0; memPlus < lastMem; memPlus+=elSize){
        memcpy((mat->supDia)+memPlus, element, elSize);
        memcpy((mat->infDia)+memPlus, element, elSize);
        memcpy((mat->princDia)+memPlus, element, elSize);
    }
    memcpy((mat->princDia)+lastMem, element, elSize);
}

void modifyTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    const void *newElement, const Algebra *scalarAlgebra){
    TriMatrix *mat = (TriMatrix*)matrix;
    unsigned int elSize = scalarAlgebra->elementSize;
    if(row == column){
        unsigned int memPlus = row*elSize;
        memcpy((mat->princDia)+memPlus, newElement, elSize);
    }else if(row == column - 1){
        unsigned int memPlus = row*elSize;
        memcpy((mat->supDia)+memPlus, newElement, elSize);
    }else if(row == column + 1){
        unsigned int memPlus = column*elSize;
        memcpy((mat->infDia)+memPlus, newElement, elSize);
    }
}

int recieveTridiagonalMatrixElement(unsigned int row, unsigned int column, const void *matrix,
    void *reciept, const Algebra *scalarAlgebra){
    TriMatrix *mat = (TriMatrix*)matrix;
    unsigned int elSize = scalarAlgebra->elementSize;
    if(row == column){
        unsigned int memPlus = row*elSize;
        memcpy(reciept, (mat->princDia)+memPlus, elSize);
    }else if(row == column - 1){
        unsigned int memPlus = row*elSize;
        memcpy(reciept, (mat->supDia)+memPlus, elSize);
    }else if(row == column + 1){
        unsigned int memPlus = column*elSize;
        memcpy(reciept, (mat->infDia)+memPlus, elSize);
    }else{
        return 0;
    }
    return 1;
}

void printTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    const Algebra *scalarAlgebra){
    TriMatrix *mat = (TriMatrix*)matrix;
    unsigned int elSize = scalarAlgebra->elementSize;
    if(row == column){
        unsigned int memPlus = row*elSize;
        scalarAlgebra->print((mat->princDia)+memPlus);
    }else if(row == column - 1){
        unsigned int memPlus = row*elSize;
        scalarAlgebra->print((mat->supDia)+memPlus);
    }else if(row == column + 1){
        unsigned int memPlus = column*elSize;
        scalarAlgebra->print((mat->infDia)+memPlus);
    }else{
        printf("0");
    }
}

int allocateMemArray(void **memArray, unsigned int matrixSize, const Algebra *scalarAlgebra){
    if(!(*(memArray) = (void*)malloc((matrixSize-1)*(scalarAlgebra->elementSize)))){
        return 0;
    }
    if(!(*(memArray+1) = (void*)malloc((matrixSize)*(scalarAlgebra->elementSize)))){
        free(*(memArray));
        return 0;
    }
    if(!(*(memArray+2) = (void*)malloc((matrixSize-1)*(scalarAlgebra->elementSize)))){
        free(*(memArray+1));
        free(*(memArray));
        return 0;
    }
    return 1;
}