#ifndef _HEADERS_AND_FUNCTIONS_H_
#define _HEADERS_AND_FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include "tridiagonal_matrix.h"
#include "integer_algebra.h"
#include "real_number_algebra.h"
#include "complex_number_algebra.h"

typedef void (*ScanModification)(void*, unsigned int, unsigned int, void*, Algebra*);
typedef void (*ScanAssignment)(void *);

void printComplexNumbersNotification(void){
    printf("(Enter by pairs of real numbers where the first number is the real part and the second number is the imaginary part without i)\n");
}

void freeMatrix(void *matrix, void ***diagonalsArray, unsigned int matrixSize){
    freeMatrixDiagonal(*(diagonalsArray), matrixSize - 1);
    freeMatrixDiagonal(*(diagonalsArray + 1), matrixSize);
    freeMatrixDiagonal(*(diagonalsArray + 2), matrixSize - 1);
    free(matrix);
}

int allocateMatrix(void **matrix, void ***diagonalsArray, unsigned int matrixSize,
    Algebra *matrixTypeAlgebra){
    if(!(allocateDiagonalsArray(diagonalsArray, matrixSize, matrixTypeAlgebra))){
        return 0;
    }
    if(!(*matrix = malloc(3*sizeof(void*)))){
        freeMatrixDiagonal(*(diagonalsArray), matrixSize - 1);
        freeMatrixDiagonal(*(diagonalsArray + 1), matrixSize);
        freeMatrixDiagonal(*(diagonalsArray + 2), matrixSize - 1);
        return 0;
    }
    createTridiagonalMatrix(diagonalsArray, *matrix);
    return 1;
}

void scanAndAssignInt(void *pointer){
    int subservient;
    scanf("%d", &subservient);
    assignInteger(pointer, subservient);
}

void scanAndAssignR(void *pointer){
    double subservient;
    scanf("%lf", &subservient);
    assignRealNumber(pointer, subservient);
}

void scanAndAssignC(void *pointer){
    double subservient1, subservient2;
    scanf("%lf", &subservient1);
    scanf("%lf", &subservient2);
    assignComplexNumber(pointer, subservient1, subservient2);
}

void scanAndModifyInt(void *pointer, unsigned int row, unsigned int column, void *matrix,
    Algebra *matrixTypeAlgebra){
    scanAndAssignInt(pointer);
    modifyTridiagonalMatrixElement(row, column, matrix, pointer, matrixTypeAlgebra);
}

void scanAndModifyR(void *pointer, unsigned int row, unsigned int column, void *matrix,
    Algebra *matrixTypeAlgebra){
    scanAndAssignR(pointer);
    modifyTridiagonalMatrixElement(row, column, matrix, pointer, matrixTypeAlgebra);
}

void scanAndModifyC(void *pointer, unsigned int row, unsigned int column, void *matrix,
    Algebra *matrixTypeAlgebra){
    scanAndAssignC(pointer);
    modifyTridiagonalMatrixElement(row, column, matrix, pointer, matrixTypeAlgebra);
}

#endif