#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scalar_algebra.h"

typedef struct _struct_TriMatrix{

    char *supDia;
    char *princDia;
    char *infDia;

    const Algebra *alg;
    size_t size;

} TriMatrix;

int addTridiagonalMatrices(const TriMatrix *m1, const TriMatrix *m2, TriMatrix *res){

    if(m1->size != m2->size || m1->size != res->size) return 0;
    if(m1->alg != m2->alg || m1->alg != res->alg) return 0;

    size_t lastMem = (m1->size - 1)*(m1->alg->elementSize);

    for(size_t memPlus = 0; memPlus < lastMem; memPlus+=(m1->alg->elementSize)){
        m1->alg->add((m1->supDia)+memPlus, (m2->supDia)+memPlus, (res->supDia)+memPlus);
        m1->alg->add((m1->infDia)+memPlus, (m2->infDia)+memPlus, (res->infDia)+memPlus);
        m1->alg->add((m1->princDia)+memPlus, (m2->princDia)+memPlus, (res->princDia)+memPlus);
    }
    m1->alg->add((m1->princDia)+lastMem, (m2->princDia)+lastMem, (res->princDia)+lastMem);

    return 1;

}

int scalarlyMultiplyTridiagonalMatrix(const void *scalar, const TriMatrix *mat, TriMatrix *res,
            const Algebra *scalarAlgebra){

    if(mat->size != res->size) return 0;
    if(mat->alg != scalarAlgebra || mat->alg != res->alg) return 0;

    size_t lastMem = (mat->size - 1)*(scalarAlgebra->elementSize);

    for(size_t memPlus = 0; memPlus < lastMem; memPlus+=(scalarAlgebra->elementSize)){
        scalarAlgebra->multiply(scalar, (mat->supDia)+memPlus, (res->supDia)+memPlus);
        scalarAlgebra->multiply(scalar, (mat->infDia)+memPlus, (res->infDia)+memPlus);
        scalarAlgebra->multiply(scalar, (mat->princDia)+memPlus, (res->princDia)+memPlus);
    }
    scalarAlgebra->multiply(scalar, (mat->princDia)+lastMem, (res->princDia)+lastMem);

    return 1;

}

int allocTridiagonalMatrix(TriMatrix **mat, size_t matrixSize, const Algebra *scalarAlgebra){

    if(!(*mat = (TriMatrix*)malloc(sizeof(TriMatrix)))) return 0;

    (*mat)->size = matrixSize;
    (*mat)->alg = scalarAlgebra;

    if(!((*mat)->princDia = malloc((matrixSize)*(scalarAlgebra->elementSize)))){
        free(*mat);
        return 0;
    }
    if(matrixSize > 1){
        if(!((*mat)->supDia = malloc((matrixSize-1)*(scalarAlgebra->elementSize)))){
            free((*mat)->princDia);
            free(*mat);
            return 0;
        }
        if(!((*mat)->infDia = malloc((matrixSize-1)*(scalarAlgebra->elementSize)))){
            free((*mat)->princDia);
            free((*mat)->supDia);
            free(*mat);
            return 0;
        }
    }

    return 1;

}

void freeTridiagonalMatrix(TriMatrix *mat){
    if(mat){

        free(mat->infDia);
        free(mat->princDia);
        free(mat->supDia);

        free(mat);

    }
}

int initializeTridiagonalMatrix(TriMatrix *mat, const void *scalar, const Algebra *scalarAlgebra){

    if(mat->alg != scalarAlgebra) return 0;

    size_t elSize = scalarAlgebra->elementSize;
    size_t lastMem = (mat->size - 1)*elSize;

    for(size_t memPlus = 0; memPlus < lastMem; memPlus+=elSize){
        memcpy((mat->supDia)+memPlus, scalar, elSize);
        memcpy((mat->infDia)+memPlus, scalar, elSize);
        memcpy((mat->princDia)+memPlus, scalar, elSize);
    }
    memcpy((mat->princDia)+lastMem, scalar, elSize);

    return 1;

}

int setTridiagonalMatrixElement(size_t row, size_t column, TriMatrix *mat,
            const void *scalar, const Algebra *scalarAlgebra){

    if(mat->alg != scalarAlgebra) return 0;

    if(row >= mat->size || column >= mat->size) return 0;

    size_t elSize = scalarAlgebra->elementSize;

    if(row == column){
        size_t memPlus = row*elSize;
        memcpy((mat->princDia)+memPlus, scalar, elSize);
    }else if(row + 1 == column){
        size_t memPlus = row*elSize;
        memcpy((mat->supDia)+memPlus, scalar, elSize);
    }else if(row == column + 1){
        size_t memPlus = column*elSize;
        memcpy((mat->infDia)+memPlus, scalar, elSize);
    }else{
        return 0;
    }

    return 1;

}

int getTridiagonalMatrixElement(size_t row, size_t column, const TriMatrix *mat,
            void *scalar, const Algebra *scalarAlgebra){

    if(mat->alg != scalarAlgebra) return 0;

    if(row >= mat->size || column >= mat->size) return 0;

    size_t elSize = scalarAlgebra->elementSize;

    if(row == column){
        size_t memPlus = row*elSize;
        memcpy(scalar, (mat->princDia)+memPlus, elSize);
    }else if(row + 1 == column){
        size_t memPlus = row*elSize;
        memcpy(scalar, (mat->supDia)+memPlus, elSize);
    }else if(row == column + 1){
        size_t memPlus = column*elSize;
        memcpy(scalar, (mat->infDia)+memPlus, elSize);
    }else{
        return 0;
    }

    return 1;

}

int scanTridiagonalMatrixElement(size_t row, size_t column, TriMatrix *mat){

    if(row >= mat->size || column >= mat->size) return -2;

    size_t elSize = mat->alg->elementSize;

    if(row == column){
        size_t memPlus = row*elSize;
        return mat->alg->scan((mat->princDia)+memPlus);
    }else if(row == column - 1){
        size_t memPlus = row*elSize;
        return mat->alg->scan((mat->supDia)+memPlus);
    }else if(row == column + 1){
        size_t memPlus = column*elSize;
        return mat->alg->scan((mat->infDia)+memPlus);
    }else{
        return -2;
    }

}

int sScanTridiagonalMatrixElement(const char *input, size_t row, size_t column, TriMatrix *mat){

    if(row >= mat->size || column >= mat->size) return -2;

    size_t elSize = mat->alg->elementSize;

    if(row == column){
        size_t memPlus = row*elSize;
        return mat->alg->sScan(input, (mat->princDia)+memPlus);
    }else if(row == column - 1){
        size_t memPlus = row*elSize;
        return mat->alg->sScan(input, (mat->supDia)+memPlus);
    }else if(row == column + 1){
        size_t memPlus = column*elSize;
        return mat->alg->sScan(input, (mat->infDia)+memPlus);
    }else{
        return -2;
    }

}

int sNScanTridiagonalMatrixElement(const char *input, size_t row, size_t column, TriMatrix *mat, int *stopIndex){
    
    if(row >= mat->size || column >= mat->size) return -2;

    size_t elSize = mat->alg->elementSize;

    if(row == column){
        size_t memPlus = row*elSize;
        return mat->alg->sNScan(input, (mat->princDia)+memPlus, stopIndex);
    }else if(row == column - 1){
        size_t memPlus = row*elSize;
        return mat->alg->sNScan(input, (mat->supDia)+memPlus, stopIndex);
    }else if(row == column + 1){
        size_t memPlus = column*elSize;
        return mat->alg->sNScan(input, (mat->infDia)+memPlus, stopIndex);
    }else{
        return -2;
    }

}

int printTridiagonalMatrixElement(size_t row, size_t column, const TriMatrix *mat){

    if(row >= mat->size || column >= mat->size) return 0;

    size_t elSize = mat->alg->elementSize;

    if(row == column){
        size_t memPlus = row*elSize;
        mat->alg->print((mat->princDia)+memPlus);
    }else if(row == column - 1){
        size_t memPlus = row*elSize;
        mat->alg->print((mat->supDia)+memPlus);
    }else if(row == column + 1){
        size_t memPlus = column*elSize;
        mat->alg->print((mat->infDia)+memPlus);
    }else{
        printf("0");
    }

    return 1;

}