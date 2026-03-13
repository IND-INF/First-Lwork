#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scalar_algebra.h"

typedef struct TriMatrix{
    void **supDia;
    void **princDia;
    void **infDia;
} TriMatrix;

void addTridiagonalMatrices(unsigned int size, const void *matrix1, const void *matrix2,
    void *result, Algebra *scalarAlgebra){
    TriMatrix *res = (TriMatrix*)result;
    const TriMatrix *m1 = (const TriMatrix*)matrix1;
    const TriMatrix *m2 = (const TriMatrix*)matrix2;
    for(unsigned int iteration = 0; iteration < size - 1; iteration++){
        scalarAlgebra->add(*((m1->supDia)+iteration), *((m2->supDia)+iteration),
            *((res->supDia)+iteration));
        scalarAlgebra->add(*((m1->infDia)+iteration), *((m2->infDia)+iteration),
            *((res->infDia)+iteration));
        scalarAlgebra->add(*((m1->princDia)+iteration), *((m2->princDia)+iteration),
            *((res->princDia)+iteration));
    }
    scalarAlgebra->add(*((m1->princDia)+(size-1)), *((m2->princDia)+(size-1)),
        *((res->princDia)+(size-1)));
}

int multiplyTridiagonalMatrices(unsigned int size, const void *matrix1, const void *matrix2,
    void *result, Algebra *scalarAlgebra){
    TriMatrix *res = (TriMatrix*)result;
    const TriMatrix *m1 = (const TriMatrix*)matrix1;
    const TriMatrix *m2 = (const TriMatrix*)matrix2;
    if(size == 1){
        scalarAlgebra->multiply(*(m1->princDia), *(m2->princDia), *(res->princDia));
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
    scalarAlgebra->multiply(*(m1->princDia), *(m2->princDia), subservient1);
    scalarAlgebra->multiply(*(m1->supDia), *(m2->infDia), subservient2);
    scalarAlgebra->add(subservient1, subservient2, *(res->princDia));
    for(unsigned int iteration = 0; iteration < size - 1; iteration++){
        scalarAlgebra->multiply(*((m1->princDia)+iteration), *((m2->supDia)+iteration),
            subservient1);
        scalarAlgebra->multiply(*((m1->supDia)+iteration), *((m2->princDia)+iteration+1),
            subservient2);
        scalarAlgebra->add(subservient1, subservient2,
            *((res->supDia)+iteration));
    }
    for(unsigned int iteration = 0; iteration < size - 1; iteration++){
        scalarAlgebra->multiply(*((m1->infDia)+iteration), *((m2->princDia)+iteration),
            subservient1);
        scalarAlgebra->multiply(*((m1->princDia)+iteration+1), *((m2->infDia)+iteration),
            subservient2);
        scalarAlgebra->add(subservient1, subservient2,
            *((res->infDia)+iteration));
    }
    for(unsigned int iteration = 0; iteration < size - 2; iteration++){
        scalarAlgebra->multiply(*((m1->infDia)+iteration), *((m2->supDia)+iteration),
            subservient1);
        scalarAlgebra->multiply(*((m1->princDia)+iteration+1), *((m2->princDia)+iteration+1),
            subservient2);
        scalarAlgebra->add(subservient1, subservient2,
            subservient1);
        scalarAlgebra->multiply(*((m1->supDia)+iteration+1), *((m2->infDia)+iteration+1),
            subservient2);
        scalarAlgebra->add(subservient1, subservient2,
            *((res->princDia)+iteration+1));
    }
    scalarAlgebra->multiply(*((m1->infDia)+(size-2)), *((m2->supDia)+(size-2)),
        subservient1);
    scalarAlgebra->multiply(*((m1->princDia)+(size-1)), *((m2->princDia)+(size-1)),
        subservient2);
    scalarAlgebra->add(subservient1, subservient2,
        *((res->princDia)+(size-1)));
    free(subservient1); free(subservient2);
    return 1;
}

void scalarlyMultiplyTridiagonalMatrix(unsigned int size, const void *scalar, const void *matrix,
    void *result, Algebra *scalarAlgebra){
    TriMatrix *res = (TriMatrix*)result;
    const TriMatrix *mat = (const TriMatrix*)matrix;
    for(unsigned int iteration = 0; iteration < size - 1; iteration++){
        scalarAlgebra->multiply(scalar, *((mat->supDia)+iteration),
            *((res->supDia)+iteration));
        scalarAlgebra->multiply(scalar, *((mat->infDia)+iteration),
            *((res->infDia)+iteration));
        scalarAlgebra->multiply(scalar, *((mat->princDia)+iteration),
            *((res->princDia)+iteration));
    }
    scalarAlgebra->multiply(scalar, *((mat->princDia)+(size-1)),
        *((res->princDia)+(size-1)));
}

void createTridiagonalMatrix(void ***diagonalsArray, void *matrix){
    TriMatrix *mat = (TriMatrix*)matrix;
    mat->supDia = *diagonalsArray;
    mat->princDia = *(diagonalsArray + 1);
    mat->infDia = *(diagonalsArray + 2);
}

void assignTridiagonalMatrix(unsigned int size, void *matrix, void *element, Algebra *scalarAlgebra){
    TriMatrix *mat = (TriMatrix*)matrix;
    for(unsigned int iteration = 0; iteration < size - 1; iteration++){
        memcpy(*((mat->supDia)+iteration), element, scalarAlgebra->elementSize);
        memcpy(*((mat->infDia)+iteration), element, scalarAlgebra->elementSize);
        memcpy(*((mat->princDia)+iteration), element, scalarAlgebra->elementSize);
    }
    memcpy(*((mat->princDia)+(size-1)), element, scalarAlgebra->elementSize);
}

void modifyTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    void *newElement, Algebra *scalarAlgebra){
    TriMatrix *mat = (TriMatrix*)matrix;
    if(row == column){
        memcpy(*((mat->princDia)+row), newElement, scalarAlgebra->elementSize);
    }else if(row == column - 1){
        memcpy(*((mat->supDia)+row), newElement, scalarAlgebra->elementSize);
    }else if(row == column + 1){
        memcpy(*((mat->infDia)+column), newElement, scalarAlgebra->elementSize);
    }
}

int recieveTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    void *reciept, Algebra *scalarAlgebra){
    TriMatrix *mat = (TriMatrix*)matrix;
    if(row == column){
        memcpy(reciept, *((mat->princDia)+row), scalarAlgebra->elementSize);
    }else if(row == column - 1){
        memcpy(reciept, *((mat->princDia)+row), scalarAlgebra->elementSize);
    }else if(row == column + 1){
        memcpy(reciept, *((mat->princDia)+row), scalarAlgebra->elementSize);
    }else{
        return 0;
    }
    return 1;
}

void printTridiagonalMatrixElement(unsigned int row, unsigned int column, void *matrix,
    Algebra *scalarAlgebra){
    TriMatrix *mat = (TriMatrix*)matrix;
    if(row == column){
        scalarAlgebra->print(*((mat->princDia)+row));
    }else if(row == column - 1){
        scalarAlgebra->print(*((mat->supDia)+row));
    }else if(row == column + 1){
        scalarAlgebra->print(*((mat->infDia)+column));
    }else{
        printf("0");
    }
}
