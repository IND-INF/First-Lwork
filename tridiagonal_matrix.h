#ifndef _TRIDIAGONAL_MATRIX_H_
#define _TRIDIAGONAL_MATRIX_H_

#include "scalar_algebra.h"

void addTridiagonalMatrices(unsigned int size, const void *matrix1, const void *matrix2,
    void *result, Algebra *scalarAlgebra);
int multiplyTridiagonalMatrices(unsigned int size, const void *matrix1, const void *matrix2,
    void *result, Algebra *scalarAlgebra);
void scalarlyMultiplyTridiagonalMatrix(unsigned int size, const void *scalar, const void *matrix,
    void *result, Algebra *scalarAlgebra);
void createTridiagonalMatrix(void ***diagonalsArray, void *matrix);
void assignTridiagonalMatrix(unsigned int size, void *matrix, void *element, Algebra *scalarAlgebra);
void modifyTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    void *newElement, Algebra *scalarAlgebra);
int recieveTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    void *reciept, Algebra *scalarAlgebra);
void printTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    Algebra *scalarAlgebra);
void freeMatrixDiagonal(void **diagonal, unsigned int diagonalLength){
    for(unsigned int number = 0; number < diagonalLength; number++){
        free(*(diagonal+number));
    }
    free(diagonal);
}
int allocateDiagonalsArray(void ***diagonalsArray, unsigned int matrixSize, Algebra *scalarAlgebra){
    if(!(*(diagonalsArray) = (void**)malloc((matrixSize-1)*sizeof(void*)))){
        return 0;
    }
    if(!(*(diagonalsArray+1) = (void**)malloc((matrixSize)*sizeof(void*)))){
        free(*(diagonalsArray));
        return 0;
    }
    if(!(*(diagonalsArray+2) = (void**)malloc((matrixSize-1)*sizeof(void*)))){
        free(*(diagonalsArray+1));
        free(*(diagonalsArray));
        return 0;
    }
    for(unsigned int number = 0; number < matrixSize - 1; number++){
        if(!(*(*(diagonalsArray)+number) = malloc(scalarAlgebra->elementSize))){
            for(number--; number >= 0; number--){
                free(*(*(diagonalsArray)+number));
            }
            free(*(diagonalsArray+2));
            free(*(diagonalsArray+1));
            free(*(diagonalsArray));
            return 0;
        }
    }
    for(unsigned int number = 0; number < matrixSize; number++){
        if(!(*(*(diagonalsArray+1)+number) = malloc(scalarAlgebra->elementSize))){
            freeMatrixDiagonal(*(diagonalsArray), matrixSize - 1);
            for(number--; number >= 0; number--){
                free(*(*(diagonalsArray+1)+number));
            }
            free(*(diagonalsArray+2));
            free(*(diagonalsArray+1));
            return 0;
        }
    }
    for(unsigned int number = 0; number < matrixSize - 1; number++){
        if(!(*(*(diagonalsArray+2)+number) = malloc(scalarAlgebra->elementSize))){
            freeMatrixDiagonal(*(diagonalsArray), matrixSize - 1);
            freeMatrixDiagonal(*(diagonalsArray + 1), matrixSize);
            for(number--; number >= 0; number--){
                free(*(*(diagonalsArray+2)+number));
            }
            free(*(diagonalsArray+2));
            return 0;
        }
    }
    return 1;
}

#endif