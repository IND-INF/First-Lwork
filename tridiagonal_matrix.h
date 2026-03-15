#ifndef _TRIDIAGONAL_MATRIX_H_
#define _TRIDIAGONAL_MATRIX_H_

#include "scalar_algebra.h"

void addTridiagonalMatrices(unsigned int size, const void *matrix1, const void *matrix2,
    void *result, const Algebra *scalarAlgebra);

int multiplyTridiagonalMatrices(unsigned int size, const void *matrix1, const void *matrix2,
    void *result, const Algebra *scalarAlgebra);

void scalarlyMultiplyTridiagonalMatrix(unsigned int size, const void *scalar, const void *matrix,
    void *result, const Algebra *scalarAlgebra);

void createTridiagonalMatrix(void **memArray, void *matrix);

void assignTridiagonalMatrix(unsigned int size, void *matrix, const void *element, const Algebra *scalarAlgebra);

void modifyTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    const void *newElement, const Algebra *scalarAlgebra);

int recieveTridiagonalMatrixElement(unsigned int row, unsigned int column, const void *matrix,
    void *reciept, const Algebra *scalarAlgebra);

void printTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    const Algebra *scalarAlgebra);

int allocateMemArray(void **memArray, unsigned int matrixSize, const Algebra *scalarAlgebra);/*
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
*/

#endif