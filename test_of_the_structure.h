#ifndef _TEST_OF_THE_STRUCTURE_H_
#define _TEST_OF_THE_STRUCTURE_H_

#include <stdio.h>
#include <stdlib.h>
#include "tridiagonal_matrix.h"
#include "integer_algebra.h"
#include "real_number_algebra.h"
#include "complex_number_algebra.h"

typedef int (*makeFunction)(double*, void*, unsigned int, void***, Algebra*);
typedef void (*makeNumFunction)(double*, void*);
typedef int (*compareFunction)(double*, void*, unsigned int, Algebra*);

void freeDiagonals(void ***diagonalsArray, unsigned int matrixSize){
    freeMatrixDiagonal(*(diagonalsArray), matrixSize - 1);
    freeMatrixDiagonal(*(diagonalsArray + 1), matrixSize);
    freeMatrixDiagonal(*(diagonalsArray + 2), matrixSize - 1);
}

int composeTridiagonal(void *matrix, unsigned int size, void ***diagonalsArray, Algebra *alg){
    if(!(allocateDiagonalsArray(diagonalsArray, size, alg))) return 0;
    createTridiagonalMatrix(diagonalsArray, matrix);
    return 1;
}

int makeRTridiagonal(double *array, void *matrix, unsigned int size, void ***diagonalsArray,
    Algebra *alg){
    if(!(composeTridiagonal(matrix, size, diagonalsArray, alg))) return 0;
    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        freeDiagonals(diagonalsArray, size);
        return 0;
    }
    for(unsigned int row = 0; row < size - 1; row++){
        assignRealNumber(ptrToElement, array[row*size+row]);
        modifyTridiagonalMatrixElement(row, row, matrix, ptrToElement, alg);
        assignRealNumber(ptrToElement, array[row*size+row+1]);
        modifyTridiagonalMatrixElement(row, row+1, matrix, ptrToElement, alg);
        assignRealNumber(ptrToElement, array[(row+1)*size+row]);
        modifyTridiagonalMatrixElement(row+1, row, matrix, ptrToElement, alg);
    }
    assignRealNumber(ptrToElement, array[(size-1)*size+size-1]);
    modifyTridiagonalMatrixElement(size-1, size-1, matrix, ptrToElement, alg);
    return 1;
}

int makeCTridiagonal(double *array, void *matrix, unsigned int size, void ***diagonalsArray,
    Algebra *alg){
    if(!(composeTridiagonal(matrix, size, diagonalsArray, alg))) return 0;
    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        freeDiagonals(diagonalsArray, size);
        return 0;
    }
    for(unsigned int row = 0; row < size - 1; row++){
        assignComplexNumber(ptrToElement, array[2*(row*size+row)],
            array[2*(row*size+row)+1]);
        modifyTridiagonalMatrixElement(row, row, matrix, ptrToElement, alg);
        assignComplexNumber(ptrToElement, array[2*(row*size+row+1)],
            array[2*(row*size+row+1)+1]);
        modifyTridiagonalMatrixElement(row, row+1, matrix, ptrToElement, alg);
        assignComplexNumber(ptrToElement, array[2*((row+1)*size+row)],
            array[2*((row+1)*size+row)+1]);
        modifyTridiagonalMatrixElement(row+1, row, matrix, ptrToElement, alg);
    }
    assignComplexNumber(ptrToElement, array[2*((size-1)*size+size-1)],
        array[2*((size-1)*size+size-1)+1]);
    modifyTridiagonalMatrixElement(size-1, size-1, matrix, ptrToElement, alg);
    return 1;
}

int makeThreeMatrices(double **arrayArr, void **matrixArr, void ****memoryArr, unsigned int size,
    void **functions){
    makeFunction makeTridiagonal = (makeFunction)(functions[1]);
    Algebra *alg = (Algebra*)(functions[0]);
    if(!(makeTridiagonal(arrayArr[0], matrixArr[0], size, memoryArr[0], alg))){
        return 0;
    }
    if(!(makeTridiagonal(arrayArr[1], matrixArr[1], size, memoryArr[1], alg))){
        freeDiagonals(memoryArr[0], size);
        return 0;
    }
    if(!(composeTridiagonal(matrixArr[2], size, memoryArr[2], alg))){
        freeDiagonals(memoryArr[0], size);
        freeDiagonals(memoryArr[1], size);
        return 0;
    }
    return 1;
}

void makeRNumber(double *array, void *ptrToElement){
    assignRealNumber(ptrToElement, array[0]);
}

void makeCNumber(double *array, void *ptrToElement){
    assignComplexNumber(ptrToElement, array[0], array[1]);
}

int compareRTridiagonal(double *array, void *matrix, unsigned int size, Algebra *alg){
    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return 0;
    double number = 0;
    for(unsigned int row = 0; row < size - 1; row++){
        recieveTridiagonalMatrixElement(row, row, matrix, ptrToElement, alg);
        recieveRealNumber(ptrToElement, &number);
        if(array[row*size+row] != number){
            free(ptrToElement);
            return 2;
        }
        recieveTridiagonalMatrixElement(row, row+1, matrix, ptrToElement, alg);
        recieveRealNumber(ptrToElement, &number);
        if(array[row*size+row+1] != number){
            free(ptrToElement);
            return 2;
        }
        recieveTridiagonalMatrixElement(row+1, row, matrix, ptrToElement, alg);
        recieveRealNumber(ptrToElement, &number);
        if(array[(row+1)*size+row] != number){
            free(ptrToElement);
            return 2;
        }
    }
    recieveTridiagonalMatrixElement(size-1, size-1, matrix, ptrToElement, alg);
    recieveRealNumber(ptrToElement, &number);
    if(array[(size-1)*size+size-1] != number){
        free(ptrToElement);
        return 2;
    }
    free(ptrToElement);
    return 1;
}

int compareCTridiagonal(double *array, void *matrix, unsigned int size, Algebra *alg){
    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return 0;
    double rePart = 0, imPart = 0;
    for(unsigned int row = 0; row < size - 1; row++){
        recieveTridiagonalMatrixElement(row, row, matrix, ptrToElement, alg);
        recieveComplexNumber(ptrToElement, &rePart, &imPart);
        if(array[2*(row*size+row)] != rePart || array[2*(row*size+row)+1] != imPart){
            free(ptrToElement);
            return 2;
        }
        recieveTridiagonalMatrixElement(row, row+1, matrix, ptrToElement, alg);
        recieveComplexNumber(ptrToElement, &rePart, &imPart);
        if(array[2*(row*size+row+1)] != rePart || array[2*(row*size+row+1)+1] != imPart){
            free(ptrToElement);
            return 2;
        }
        recieveTridiagonalMatrixElement(row+1, row, matrix, ptrToElement, alg);
        recieveComplexNumber(ptrToElement, &rePart, &imPart);
        if(array[2*((row+1)*size+row)] != rePart || array[2*((row+1)*size+row)+1] != imPart){
            free(ptrToElement);
            return 2;
        }
    }
    recieveTridiagonalMatrixElement(size-1, size-1, matrix, ptrToElement, alg);
    recieveComplexNumber(ptrToElement, &rePart, &imPart);
    if(array[2*((size-1)*size+size-1)] != rePart || array[2*((size-1)*size+size-1)+1] != imPart){
        free(ptrToElement);
        return 2;
    }
    free(ptrToElement);
    return 1;
}

int checkAdding(double **arrayArr, void **matrixArr, void ****memoryArr, unsigned int size,
    void **functions){
    Algebra *alg = (Algebra*)(functions[0]);
    if(!(makeThreeMatrices(arrayArr, matrixArr, memoryArr, size, functions))){
        return 0;
    }
    compareFunction compareTridiagonal = (compareFunction)(functions[3]);
    addTridiagonalMatrices(size, matrixArr[0], matrixArr[1], matrixArr[2], alg);
    int output = compareTridiagonal(arrayArr[2], matrixArr[2], size, alg);
    freeDiagonals(memoryArr[0], size);
    freeDiagonals(memoryArr[1], size);
    freeDiagonals(memoryArr[2], size);
    return output;
}

int checkMult(double **arrayArr, void **matrixArr, void ****memoryArr, unsigned int size,
    void **functions){
    Algebra *alg = (Algebra*)(functions[0]);
    if(!(makeThreeMatrices(arrayArr, matrixArr, memoryArr, size, functions))){
        return 0;
    }
    if(!(multiplyTridiagonalMatrices(size, matrixArr[0], matrixArr[1], matrixArr[2], alg))){
        freeDiagonals(memoryArr[0], size);
        freeDiagonals(memoryArr[1], size);
        freeDiagonals(memoryArr[2], size);
        return 0;
    }
    compareFunction compareTridiagonal = (compareFunction)(functions[3]);
    int output = compareTridiagonal(arrayArr[2], matrixArr[2], size, alg);
    freeDiagonals(memoryArr[0], size);
    freeDiagonals(memoryArr[1], size);
    freeDiagonals(memoryArr[2], size);
    return output;
}

int checkScalMult(double **arrayArr, void **matrixArr, void ****memoryArr, unsigned int size,
    void **functions){
    Algebra *alg = (Algebra*)(functions[0]);
    makeFunction makeTridiagonal = (makeFunction)(functions[1]);
    makeNumFunction makeNumber = (makeNumFunction)(functions[2]);
    compareFunction compareTridiagonal = (compareFunction)(functions[3]);
    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        return 0;
    }
    makeNumber(arrayArr[0], ptrToElement);
    if(!(makeTridiagonal(arrayArr[1], matrixArr[1], size, memoryArr[1], alg))){
        free(ptrToElement);
        return 0;
    }
    if(!(composeTridiagonal(matrixArr[2], size, memoryArr[2], alg))){
        free(ptrToElement);
        freeDiagonals(memoryArr[1], size);
        return 0;
    }
    scalarlyMultiplyTridiagonalMatrix(size, ptrToElement, matrixArr[1], matrixArr[2], alg);
    int output = compareTridiagonal(arrayArr[2], matrixArr[2], size, alg);
    free(ptrToElement);
    freeDiagonals(memoryArr[1], size);
    freeDiagonals(memoryArr[2], size);
    return output;
}

int Test1(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double fstMat[] = {5};
    double sndMat[] = {3};
    double resMat[] = {8};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test2(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double fstMat[] = {
        0, 0,
        0, 0
    };
    double sndMat[] = {
        0, 0,
        0, 0
    };
    double resMat[] = {
        0, 0,
        0, 0
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test3(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double fstMat[] = {
        1, 0,
        0, 2
    };
    double sndMat[] = {
        3, 0,
        0, 4
    };
    double resMat[] = {
        4, 0,
        0, 6
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test4(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        1, 2, 0,
        3, 4, 5,
        0, 6, 7
    };
    double sndMat[] = {
        7, 8, 0,
        9, 10, 11,
        0, 12, 13
    };
    double resMat[] = {
        8, 10, 0,
        12, 14, 16,
        0, 18, 20
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test5(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    double sndMat[] = {
        1, 2, 0,
        3, 4, 5,
        0, 6, 7
    };
    double resMat[] = {
        1, 2, 0,
        3, 4, 5,
        0, 6, 7
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test6(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 4;
    double fstMat[] = {
        1, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 3, 0,
        0, 0, 0, 4
    };
    double sndMat[] = {
        4, 0, 0, 0,
        0, 3, 0, 0,
        0, 0, 2, 0,
        0, 0, 0, 1
    };
    double resMat[] = {
        5, 0, 0, 0,
        0, 5, 0, 0,
        0, 0, 5, 0,
        0, 0, 0, 5
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test7(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 5;
    double fstMat[] = {
        1, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 1
    };
    double sndMat[] = {
        2, 0, 0, 0, 0,
        0, 2, 0, 0, 0,
        0, 0, 2, 0, 0,
        0, 0, 0, 2, 0,
        0, 0, 0, 0, 2
    };
    double resMat[] = {
        3, 0, 0, 0, 0,
        0, 3, 0, 0, 0,
        0, 0, 3, 0, 0,
        0, 0, 0, 3, 0,
        0, 0, 0, 0, 3
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test8(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double fstMat[] = {0};
    double sndMat[] = {-1};
    double resMat[] = {-1};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test9(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double fstMat[] = {2};
    double sndMat[] = {3};
    double resMat[] = {6};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test10(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double fstMat[] = {
        1, 2,
        3, 4
    };
    double sndMat[] = {
        5, 6,
        7, 8
    };
    double resMat[] = {
        19, 22,
        43, 50
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test11(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    double sndMat[] = {
        1, 2, 0,
        3, 4, 5,
        0, 6, 7
    };
    double resMat[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test12(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        1, 0, 0,
        0, 2, 0,
        0, 0, 3
    };
    double sndMat[] = {
        4, 0, 0,
        0, 5, 0,
        0, 0, 6
    };
    double resMat[] = {
        4, 0, 0,
        0, 10, 0,
        0, 0, 18
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test13(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 4;
    double fstMat[] = {
        1, 2, 0, 0,
        3, 4, 5, 0,
        0, 6, 7, 8,
        0, 0, 9, 10
    };
    double sndMat[] = {
        10, 11, 0, 0,
        12, 13, 14, 0,
        0, 15, 16, 17,
        0, 0, 18, 19
    };
    double resMat[] = {
        34, 37, 0, 0,
        78, 160, 136, 0,
        0, 183, 340, 271,
        0, 0, 324, 343
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test14(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double fstMat[] = {
        -1, 2,
        3, -4
    };
    double sndMat[] = {
        5, -6,
        7, 8
    };
    double resMat[] = {
        9, 22,
        -13, -50
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test15(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 5;
    double fstMat[] = {
        1, 1, 0, 0, 0,
        1, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 
        0, 0, 1, 1, 1,
        0, 0, 0, 1, 1
    };
    double sndMat[] = {
        1, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 1
    };
    double resMat[] = {
        1, 1, 0, 0, 0,
        1, 1, 1, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 1, 1, 1,
        0, 0, 0, 1, 1
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test16(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    double sndMat[] = {
        2, 3, 0,
        4, 5, 6,
        0, 7, 8
    };
    double resMat[] = {
        2, 3, 0,
        4, 5, 6,
        0, 7, 8
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test17(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double scal[] = {2};
    double mat[] = {5};
    double resMat[] = {10};
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test18(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double scal[] = {3};
    double mat[] = {
        0, 0,
        0, 0
    };
    double resMat[] = {
        0, 0,
        0, 0
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test19(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double scal[] = {-1};
    double mat[] = {
        1, 2, 0,
        3, 4, 5,
        0, 6, 7
    };
    double resMat[] = {
        -1, -2, 0,
        -3, -4, -5,
        0, -6, -7
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test20(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 4;
    double scal[] = {0};
    double mat[] = {
        1, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 3, 0,
        0, 0, 0, 4
    };
    double resMat[] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test21(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 5;
    double scal[] = {1.5};
    double mat[] = {
        1, 1, 0, 0, 0,
        1, 2, 1, 0, 0,
        0, 1, 3, 1, 0,
        0, 0, 1, 4, 1,
        0, 0, 0, 1, 5
    };
    double resMat[] = {
        1.5, 1.5, 0, 0, 0,
        1.5, 3, 1.5, 0, 0,
        0, 1.5, 4.5, 1.5, 0,
        0, 0, 1.5, 6, 1.5,
        0, 0, 0, 1.5, 7.5
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test22(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double scal[] = {0.5};
    double mat[] = {
        -1, 2,
        3, 4
    };
    double resMat[] = {
        -0.5, 1,
        1.5, 2
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test23(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double scal[] = {-2};
    double mat[] = {-3};
    double resMat[] = {6};
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test24(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double scal[] = {4};
    double mat[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    double resMat[] = {
        4, 0, 0,
        0, 4, 0,
        0, 0, 4
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test25(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double fstMat[] = {2,3};
    double sndMat[] = {1,1};
    double resMat[] = {3,4};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test26(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double fstMat[] = {
        0,0, 0,0,
        0,0, 0,0
    };
    double sndMat[] = {
        0,0, 0,0,
        0,0, 0,0
    };
    double resMat[] = {
        0,0, 0,0,
        0,0, 0,0
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test27(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double fstMat[] = {
        1,1, 0,0,
        0,0, 2,2
    };
    double sndMat[] = {
        3,3, 0,0,
        0,0, 4,4
    };
    double resMat[] = {
        4,4, 0,0,
        0,0, 6,6
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test28(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        1,1, 2,2, 0,0,
        3,3, 4,4, 5,5,
        0,0, 6,6, 7,7
    };
    double sndMat[] = {
        7,7, 8,8, 0,0,
        9,9, 10,10, 11,11,
        0,0, 12,12, 13,13
    };
    double resMat[] = {
        8,8, 10,10, 0,0,
        12,12, 14,14, 16,16,
        0,0, 18,18, 20,20
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test29(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        0,0, 0,0, 0,0,
        0,0, 0,0, 0,0,
        0,0, 0,0, 0,0
    };
    double sndMat[] = {
        1,1, 2,2, 0,0,
        3,3, 4,4, 5,5,
        0,0, 6,6, 7,7
    };
    double resMat[] = {
        1,1, 2,2, 0,0,
        3,3, 4,4, 5,5,
        0,0, 6,6, 7,7
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test30(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 4;
    double fstMat[] = {
        1,1, 0,0, 0,0, 0,0,
        0,0, 2,2, 0,0, 0,0,
        0,0, 0,0, 3,3, 0,0,
        0,0, 0,0, 0,0, 4,4
    };
    double sndMat[] = {
        4,4, 0,0, 0,0, 0,0,
        0,0, 3,3, 0,0, 0,0,
        0,0, 0,0, 2,2, 0,0,
        0,0, 0,0, 0,0, 1,1
    };
    double resMat[] = {
        5,5, 0,0, 0,0, 0,0,
        0,0, 5,5, 0,0, 0,0,
        0,0, 0,0, 5,5, 0,0,
        0,0, 0,0, 0,0, 5,5
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test31(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double fstMat[] = {0,1};
    double sndMat[] = {-1,0};
    double resMat[] = {-1,1};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test32(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 5;
    double fstMat[] = {
        1,0, 0,1, 0,0, 0,0, 0,0,
        1,1, 2,0, 1,2, 0,0, 0,0,
        0,0, 1,0, 3,1, 0,3, 0,0,
        0,0, 0,0, 1,2, 4,0, 1,1,
        0,0, 0,0, 0,0, 1,3, 5,0
    };
    double sndMat[] = {
        0,1, 1,0, 0,0, 0,0, 0,0,
        0,2, 0,3, 2,0, 0,0, 0,0,
        0,0, 0,1, 0,2, 3,0, 0,0,
        0,0, 0,0, 2,1, 0,4, 0,1,
        0,0, 0,0, 0,0, 3,2, 0,5
    };
    double resMat[] = {
        1,1, 1,1, 0,0, 0,0, 0,0,
        1,3, 2,3, 3,2, 0,0, 0,0,
        0,0, 1,1, 3,3, 3,3, 0,0,
        0,0, 0,0, 3,3, 4,4, 1,2,
        0,0, 0,0, 0,0, 4,5, 5,5
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test33(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double fstMat[] = {1,2};
    double sndMat[] = {3,4};
    double resMat[] = {-5,10};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test34(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double fstMat[] = {
        1,1, 2,2,
        3,3, 4,4
    };
    double sndMat[] = {
        5,5, 6,6,
        7,7, 8,8
    };
    double resMat[] = {
        0,38, 0,44,
        0,86, 0,100
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test35(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        0,0, 0,0, 0,0,
        0,0, 0,0, 0,0,
        0,0, 0,0, 0,0
    };
    double sndMat[] = {
        1,1, 2,2, 0,0,
        3,3, 4,4, 5,5,
        0,0, 6,6, 7,7
    };
    double resMat[] = {
        0,0, 0,0, 0,0,
        0,0, 0,0, 0,0,
        0,0, 0,0, 0,0
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test36(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        1,0, 0,0, 0,0,
        0,0, 2,0, 0,0,
        0,0, 0,0, 3,0
    };
    double sndMat[] = {
        4,1, 0,0, 0,0,
        0,0, 5,2, 0,0,
        0,0, 0,0, 6,3
    };
    double resMat[] = {
        4,1, 0,0, 0,0,
        0,0, 10,4, 0,0,
        0,0, 0,0, 18,9
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test37(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double fstMat[] = {
        -1,0, 2,0,
        3,0, -4,0
    };
    double sndMat[] = {
        5,5, -6,6,
        7,7, 8,8
    };
    double resMat[] = {
        9,9, 22,10,
        -13,-13, -50,-14
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test38(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 4;
    double fstMat[] = {
        1,0, 0,0, 0,0, 0,0,
        0,0, 1,0, 0,0, 0,0,
        0,0, 0,0, 1,0, 0,0,
        0,0, 0,0, 0,0, 1,0
    };
    double sndMat[] = {
        1,1, 2,2, 0,0, 0,0,
        3,3, 4,4, 5,5, 0,0,
        0,0, 6,6, 7,7, 8,8,
        0,0, 0,0, 9,9, 10,10
    };
    double resMat[] = {
        1,1, 2,2, 0,0, 0,0,
        3,3, 4,4, 5,5, 0,0,
        0,0, 6,6, 7,7, 8,8,
        0,0, 0,0, 9,9, 10,10
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test39(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double fstMat[] = {
        1,1, 1,0, 0,0,
        0,1, 1,1, 1,0,
        0,0, 0,1, 1,1
    };
    double sndMat[] = {
        1,0, 1,1, 0,0,
        1,0, 1,0, 1,1,
        0,0, 1,0, 1,0
    };
    double resMat[] = {
        2,1, 1,2, 0,0,
        1,2, 1,2, 1,2,
        0,0, 1,2, 0,2
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test40(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 5;
    double fstMat[] = {
        1,1, 0,0, 0,0, 0,0, 0,0,
        0,0, 2,2, 0,0, 0,0, 0,0,
        0,0, 0,0, 3,3, 0,0, 0,0,
        0,0, 0,0, 0,0, 4,4, 0,0,
        0,0, 0,0, 0,0, 0,0, 5,5
    };
    double sndMat[] = {
        2,0, 0,0, 0,0, 0,0, 0,0,
        0,0, 3,0, 0,0, 0,0, 0,0,
        0,0, 0,0, 4,0, 0,0, 0,0,
        0,0, 0,0, 0,0, 5,0, 0,0,
        0,0, 0,0, 0,0, 0,0, 6,0
    };
    double resMat[] = {
        2,2, 0,0, 0,0, 0,0, 0,0,
        0,0, 6,6, 0,0, 0,0, 0,0,
        0,0, 0,0, 12,12, 0,0, 0,0,
        0,0, 0,0, 0,0, 20,20, 0,0,
        0,0, 0,0, 0,0, 0,0, 30,30
    };
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test41(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double scal[] = {3,4};
    double mat[] = {1,2};
    double resMat[] = {-5,10};
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test42(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double scal[] = {1,1};
    double mat[] = {
        0,0, 0,0,
        0,0, 0,0
    };
    double resMat[] = {
        0,0, 0,0,
        0,0, 0,0
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test43(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 3;
    double scal[] = {1,1};
    double mat[] = {
        1,1, 2,2, 0,0,
        3,3, 4,4, 5,5,
        0,0, 6,6, 7,7,
    };
    double resMat[] = {
        0,2, 0,4, 0,0,
        0,6, 0,8, 0,10,
        0,0, 0,12, 0,14
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test44(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 4;
    double scal[] = {0,1};
    double mat[] = {
        1,0, 1,1, 0,0, 0,0,
        1,2, 2,0, 1,3, 0,0,
        0,0, 1,0, 3,1, 1,4,
        0,0, 0,0, 1,2, 4,0
    };
    double resMat[] = {
        0,1, -1,1, 0,0, 0,0,
        -2,1, 0,-2, -3,1, 0,0,
        0,0, 0,1, -1,3, -4,1,
        0,0, 0,0, -2,1, 0,4
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test45(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double scal[] = {};
    double mat[] = {};
    double resMat[] = {};
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test46(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double scal[] = {5,5};
    double mat[] = {0,0};
    double resMat[] = {0,0};
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test47(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 2;
    double scal[] = {0.5,0};
    double mat[] = {
        -1,2, 3,4,
        5,6, 7,8
    };
    double resMat[] = {
        -0.5,1, 1.5,2,
        2.5,3, 3.5,4
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}
int Test48(void **matrixArr, void ****memoryArr, void **functions){
    unsigned int size = 1;
    double scal[] = {1,0};
    double mat[] = {
        0,1, 0,2, 0,0,
        0,3, 0,4, 0,5,
        0,0, 0,6, 0,7
    };
    double resMat[] = {
        0,1, 0,2, 0,0,
        0,3, 0,4, 0,5,
        0,0, 0,6, 0,7
    };
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memoryArr, size, functions);
}

void counter(int testReturn, int *passed, int *badAlloc, int *testsCnt){
    if(testReturn == 2){
        (*passed)--;
        printf("%d failed WA\n", *testsCnt);
    }else if(testReturn == 0){
        (*passed)--;
        (*badAlloc)++;
        printf("%d failed BadAlloc\n", *testsCnt);
    }
    (*testsCnt)++;
}

int doTheTest(void){
    int testsCnt = 1;
    int passed = 48;
    int badAlloc = 0;
    void ***diagonalsArray1;
    if(!(diagonalsArray1 = (void***)malloc(3*sizeof(void*)))){
        return 0;
    }
    void *matrix1;
    if(!(matrix1 = malloc(3*sizeof(void*)))){
        return 0;
    }
    void ***diagonalsArray2;
    if(!(diagonalsArray2 = (void***)malloc(3*sizeof(void*)))){
        return 0;
    }
    void *matrix2;
    if(!(matrix2 = malloc(3*sizeof(void*)))){
        return 0;
    }
    void ***diagonalsArray3;
    if(!(diagonalsArray3 = (void***)malloc(3*sizeof(void*)))){
        return 0;
    }
    void *matrix3;
    if(!(matrix3 = malloc(3*sizeof(void*)))){
        return 0;
    }
    void* matrixArr[] = {matrix1, matrix2, matrix3};
    void*** memoryArr[] = {diagonalsArray1, diagonalsArray2, diagonalsArray3};
    Algebra *rAlg = (Algebra*)getRealNumberAlgebra();
    Algebra *cAlg = (Algebra*)getComplexNumberAlgebra();
    void* functions[] = {(void*)rAlg, (void*)makeRTridiagonal, (void*)makeRNumber,
        (void*)compareRTridiagonal};
    void* functionsC[] = {(void*)cAlg, (void*)makeCTridiagonal, (void*)makeCNumber,
        (void*)compareCTridiagonal};
    counter(Test1(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test2(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test3(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test4(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test5(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test6(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test7(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test8(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test9(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test10(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test11(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test12(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test13(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test14(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test15(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test16(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test17(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test18(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test19(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test20(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test21(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test22(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test23(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test24(matrixArr, memoryArr, functions), &passed, &badAlloc, &testsCnt);
    counter(Test25(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test26(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test27(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test28(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test29(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test30(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test31(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test32(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test33(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test34(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test35(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test36(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test37(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test38(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test39(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test40(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test41(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test42(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test43(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test44(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test45(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test46(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test47(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    counter(Test48(matrixArr, memoryArr, functionsC), &passed, &badAlloc, &testsCnt);
    printf("%d of 48 tests passed successfully, %d of not passed tests failed because of bad memory allocation\n",
        passed, badAlloc);
    free(matrix1);
    free(matrix2);
    free(matrix3);
    free(diagonalsArray1);
    free(diagonalsArray2);
    free(diagonalsArray3);
    return 1;
}

#endif