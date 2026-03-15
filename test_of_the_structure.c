#include <stdio.h>
#include <stdlib.h>
#include "tridiagonal_matrix.h"
#include "integer_algebra.h"
#include "real_number_algebra.h"
#include "complex_number_algebra.h"
#define MACRO_FOR_CHECK_R_NUM_OPERATION(opFunction, checkRNumOpName) \
int checkRNumOpName(void **ptrArr, double *scalArr, const Algebra *alg){ \
    if(!(allocPtrArr(ptrArr, alg))) return 0; \
    assignRealNumber(ptrArr[0], scalArr[0]); \
    assignRealNumber(ptrArr[1], scalArr[1]); \
    opFunction(ptrArr[0], ptrArr[1], ptrArr[2]); \
    double ans; \
    recieveRealNumber(ptrArr[2], &ans); \
    freePtrArr(ptrArr); \
    if(ans != scalArr[2]){ \
        return 2; \
    } \
    return 1; \
}
#define MACRO_FOR_CHECK_C_NUM_OPERATION(opFunction, checkCNumOpName) \
int checkCNumOpName(void **ptrArr, double *scalArr, const Algebra *alg){ \
    if(!(allocPtrArr(ptrArr, alg))) return 0; \
    assignComplexNumber(ptrArr[0], scalArr[0], scalArr[1]); \
    assignComplexNumber(ptrArr[1], scalArr[2], scalArr[3]); \
    opFunction(ptrArr[0], ptrArr[1], ptrArr[2]); \
    double ansR, ansI; \
    recieveComplexNumber(ptrArr[2], &ansR, &ansI); \
    freePtrArr(ptrArr); \
    if(ansR != scalArr[4] || ansI != scalArr[5]){ \
        return 2; \
    } \
    return 1; \
}

typedef int (*makeFunction)(double*, void*, unsigned int, void**, const Algebra*);
typedef void (*makeNumFunction)(double*, void*);
typedef int (*compareFunction)(double*, void*, unsigned int, const Algebra*);

int checkRNumReciept(void* ptrToElement, double scal, const Algebra *alg){
    if(!(ptrToElement = malloc(alg->elementSize))) return 0;
    double reciept;
    assignRealNumber(ptrToElement, scal);
    recieveRealNumber(ptrToElement, &reciept);
    if(reciept != scal){
        free(ptrToElement);
        return 2;
    }
    recieveRealNumber(ptrToElement, &reciept);
    if(reciept != scal){
        free(ptrToElement);
        return 2;
    }
    free(ptrToElement);
    return 1;
}

int checkCNumReciept(void *ptrToElement, double rPart, double iPart, const Algebra *alg){
    if(!(ptrToElement = malloc(alg->elementSize))) return 0;
    double recieptR, recieptI;
    assignComplexNumber(ptrToElement, rPart, iPart);
    recieveComplexNumber(ptrToElement, &recieptR, &recieptI);
    if(recieptR != rPart || recieptI != iPart){
        free(ptrToElement);
        return 2;
    }
    recieveComplexNumber(ptrToElement, &recieptR, &recieptI);
    if(recieptR != rPart || recieptI != iPart){
        free(ptrToElement);
        return 2;
    }
    free(ptrToElement);
    return 1;
}

int allocPtrArr(void **ptrArr, const Algebra *alg){
    if(!(ptrArr[0] = malloc(alg->elementSize))) return 0;
    if(!(ptrArr[1] = malloc(alg->elementSize))){
        free(ptrArr[0]);
        return 0;
    }
    if(!(ptrArr[2] = malloc(alg->elementSize))){
        free(ptrArr[1]);
        free(ptrArr[0]);
        return 0;
    }
    return 1;
}

void freePtrArr(void **ptrArr){
    free(ptrArr[2]);
    free(ptrArr[1]);
    free(ptrArr[0]);
}

MACRO_FOR_CHECK_R_NUM_OPERATION(alg->add, checkRNumAddition);

MACRO_FOR_CHECK_C_NUM_OPERATION(alg->add, checkCNumAddition);

MACRO_FOR_CHECK_R_NUM_OPERATION(alg->multiply, checkRNumMult);

MACRO_FOR_CHECK_C_NUM_OPERATION(alg->multiply, checkCNumMult);

void freeMemory(void **memArray){
    free(*(memArray));
    free(*(memArray+1));
    free(*(memArray+2));
}

int composeTridiagonal(void *matrix, unsigned int size, void **memArray, const Algebra *alg){
    if(!(allocateMemArray(memArray, size, alg))) return 0;
    createTridiagonalMatrix(memArray, matrix);
    return 1;
}

int makeRTridiagonal(double *array, void *matrix, unsigned int size, void **memArray,
    const Algebra *alg){
    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return 0;
    if(!(composeTridiagonal(matrix, size, memArray, alg))){
        free(ptrToElement);
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
    free(ptrToElement);
    return 1;
}

int makeThreeMatrices(double **arrayArr, void **matrixArr, void ***memArrayArr, unsigned int size,
    const Algebra *alg){
    if(!(makeRTridiagonal(arrayArr[0], matrixArr[0], size, memArrayArr[0], alg))){
        return 0;
    }
    if(!(makeRTridiagonal(arrayArr[1], matrixArr[1], size, memArrayArr[1], alg))){
        freeMemory(memArrayArr[0]);
        return 0;
    }
    if(!(composeTridiagonal(matrixArr[2], size, memArrayArr[2], alg))){
        freeMemory(memArrayArr[0]);
        freeMemory(memArrayArr[1]);
        return 0;
    }
    return 1;
}

void makeRNumber(double *array, void *ptrToElement){
    assignRealNumber(ptrToElement, array[0]);
}

int compareRTridiagonal(double *array, void *matrix, unsigned int size, const Algebra *alg){
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

int checkAdding(double **arrayArr, void **matrixArr, void ***memArrayArr, unsigned int size,
    const Algebra *alg){
    if(!(makeThreeMatrices(arrayArr, matrixArr, memArrayArr, size, alg))){
        return 0;
    }
    addTridiagonalMatrices(size, matrixArr[0], matrixArr[1], matrixArr[2], alg);
    int output = compareRTridiagonal(arrayArr[2], matrixArr[2], size, alg);
    freeMemory(memArrayArr[0]);
    freeMemory(memArrayArr[1]);
    freeMemory(memArrayArr[2]);
    return output;
}

int checkMult(double **arrayArr, void **matrixArr, void ***memArrayArr, unsigned int size,
    const Algebra *alg){
    if(!(makeThreeMatrices(arrayArr, matrixArr, memArrayArr, size, alg))){
        return 0;
    }
    if(!(multiplyTridiagonalMatrices(size, matrixArr[0], matrixArr[1], matrixArr[2], alg))){
        freeMemory(memArrayArr[0]);
        freeMemory(memArrayArr[1]);
        freeMemory(memArrayArr[2]);
        return 0;
    }
    int output = compareRTridiagonal(arrayArr[2], matrixArr[2], size, alg);
    freeMemory(memArrayArr[0]);
    freeMemory(memArrayArr[1]);
    freeMemory(memArrayArr[2]);
    return output;
}

int checkScalMult(double **arrayArr, void **matrixArr, void ***memArrayArr, unsigned int size,
    const Algebra *alg){
    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        return 0;
    }
    makeRNumber(arrayArr[0], ptrToElement);
    if(!(makeRTridiagonal(arrayArr[1], matrixArr[1], size, memArrayArr[1], alg))){
        free(ptrToElement);
        return 0;
    }
    if(!(composeTridiagonal(matrixArr[2], size, memArrayArr[2], alg))){
        free(ptrToElement);
        freeMemory(memArrayArr[1]);
        return 0;
    }
    scalarlyMultiplyTridiagonalMatrix(size, ptrToElement, matrixArr[1], matrixArr[2], alg);
    int output = compareRTridiagonal(arrayArr[2], matrixArr[2], size, alg);
    free(ptrToElement);
    freeMemory(memArrayArr[1]);
    freeMemory(memArrayArr[2]);
    return output;
}

int ScalTest1(void *ptrToElement, const Algebra *alg){
    double scal = 1;
    return checkRNumReciept(ptrToElement, scal, alg);
}
int ScalTest2(void *ptrToElement, const Algebra *alg){
    double scal = 0;
    return checkRNumReciept(ptrToElement, scal, alg);
}
int ScalTest3(void *ptrToElement, const Algebra *alg){
    double scal = -1;
    return checkRNumReciept(ptrToElement, scal, alg);
}
int ScalTest4(void *ptrToElement, const Algebra *alg){
    double scal = 1.5;
    return checkRNumReciept(ptrToElement, scal, alg);
}
int ScalTest5(void *ptrToElement, const Algebra *alg){
    double scal = -1.5;
    return checkRNumReciept(ptrToElement, scal, alg);
}
int ScalTest6(void *ptrToElement, const Algebra *alg){
    double scal = 0.25;
    return checkRNumReciept(ptrToElement, scal, alg);
}
int ScalTest7(void *ptrToElement, const Algebra *alg){
    double scal = -15;
    return checkRNumReciept(ptrToElement, scal, alg);
}
int ScalTest8(void *ptrToElement, const Algebra *alg){
    double scal = 1000;
    return checkRNumReciept(ptrToElement, scal, alg);
}
int ScalTest9(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        3,
        2,
        5
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int ScalTest10(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,
        0,
        0
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int ScalTest11(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -1,
        -6,
        -7
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int ScalTest12(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        3,
        -5,
        -2
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int ScalTest13(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1.5,
        2,
        3.5
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int ScalTest14(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        2.5,
        -1.5,
        1
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int ScalTest15(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        2,
        -2,
        0
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int ScalTest16(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1.25,
        0.5,
        1.75
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int ScalTest17(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        3,
        2,
        6
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int ScalTest18(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1,
        5,
        5
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int ScalTest19(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,
        0,
        0
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int ScalTest20(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        2,
        -1,
        -2
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int ScalTest21(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0.5,
        5,
        2.5
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int ScalTest22(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -4,
        1.5,
        -6
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int ScalTest23(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,
        100,
        0
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int ScalTest24(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0.5,
        4.5,
        2.25
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int ScalTest25(void *ptrToElement, const Algebra *alg){
    double rPart = 1, iPart = 1;
    return checkCNumReciept(ptrToElement, rPart, iPart, alg);
}
int ScalTest26(void *ptrToElement, const Algebra *alg){
    double rPart = 0, iPart = 0;
    return checkCNumReciept(ptrToElement, rPart, iPart, alg);
}
int ScalTest27(void *ptrToElement, const Algebra *alg){
    double rPart = 2, iPart = 0;
    return checkCNumReciept(ptrToElement, rPart, iPart, alg);
}
int ScalTest28(void *ptrToElement, const Algebra *alg){
    double rPart = 0, iPart = -3;
    return checkCNumReciept(ptrToElement, rPart, iPart, alg);
}
int ScalTest29(void *ptrToElement, const Algebra *alg){
    double rPart = -4, iPart = 5;
    return checkCNumReciept(ptrToElement, rPart, iPart, alg);
}
int ScalTest30(void *ptrToElement, const Algebra *alg){
    double rPart = 0.5, iPart = 1.5;
    return checkCNumReciept(ptrToElement, rPart, iPart, alg);
}
int ScalTest31(void *ptrToElement, const Algebra *alg){
    double rPart = -2.5, iPart = 100;
    return checkCNumReciept(ptrToElement, rPart, iPart, alg);
}
int ScalTest32(void *ptrToElement, const Algebra *alg){
    double rPart = 0.25, iPart = -3.5;
    return checkCNumReciept(ptrToElement, rPart, iPart, alg);
}
int ScalTest33(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        3,4,
        5,6,
        8,10
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int ScalTest34(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,0,
        2,3,
        2,3
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int ScalTest35(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        5,6,
        -2,-1,
        3,5
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int ScalTest36(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1,0.5,
        1.25,0.25,
        2.25,0.75
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int ScalTest37(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,0,
        0,0,
        0,0
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int ScalTest38(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -5,-6,
        -7,-8,
        -12,-14
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int ScalTest39(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -5.5,6.5,
        7.5,-8.5,
        2,-2
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int ScalTest40(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        5.5,6.5,
        -7.25,-8.25,
        -1.75,-1.75
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int ScalTest41(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1,0,
        1,0,
        1,0
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int ScalTest42(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,1,
        0,1,
        -1,0
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int ScalTest43(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,0,
        0,0,
        0,0
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int ScalTest44(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        2,3,
        4,5,
        -7,22
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int ScalTest45(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        5,6,
        0,0,
        0,0
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int ScalTest46(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -1,2,
        3,-4,
        5,10
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int ScalTest47(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0.5,1.5,
        2.5,3.5,
        -4,5.5
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int ScalTest48(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0.25,1,
        -1,2,
        -2.25,-0.5
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}

int Test1(void **matrixArr, void ***memArrayArr, const Algebra *alg){
    unsigned int size = 1;
    double fstMat[] = {5};
    double sndMat[] = {3};
    double resMat[] = {8};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test2(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkAdding(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test3(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkAdding(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test4(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkAdding(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test5(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkAdding(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test6(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkAdding(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test7(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkAdding(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test8(void **matrixArr, void ***memArrayArr, const Algebra *alg){
    unsigned int size = 1;
    double fstMat[] = {0};
    double sndMat[] = {-1};
    double resMat[] = {-1};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test9(void **matrixArr, void ***memArrayArr, const Algebra *alg){
    unsigned int size = 1;
    double fstMat[] = {2};
    double sndMat[] = {3};
    double resMat[] = {6};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test10(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test11(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test12(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test13(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test14(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test15(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test16(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test17(void **matrixArr, void ***memArrayArr, const Algebra *alg){
    unsigned int size = 1;
    double scal[] = {2};
    double mat[] = {5};
    double resMat[] = {10};
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test18(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkScalMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test19(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkScalMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test20(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkScalMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test21(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkScalMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test22(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkScalMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test23(void **matrixArr, void ***memArrayArr, const Algebra *alg){
    unsigned int size = 1;
    double scal[] = {-2};
    double mat[] = {-3};
    double resMat[] = {6};
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, memArrayArr, size, alg);
}
int Test24(void **matrixArr, void ***memArrayArr, const Algebra *alg){
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
    return checkScalMult(arrayArr, matrixArr, memArrayArr, size, alg);
}

void counter(int testReturn, int *passed, int *badAlloc, int *testsCnt){
    if(testReturn == 2){
        printf("%d failed WA\n", *testsCnt);
    }else if(testReturn == 0){
        (*badAlloc)++;
        printf("%d failed BadAlloc\n", *testsCnt);
    }else (*passed)++;
    (*testsCnt)++;
}

int doTheTest(void){
    int testsCnt = 1;
    int passed = 0;
    int badAlloc = 0;
    void **memArray1;
    if(!(memArray1 = (void**)malloc(3*sizeof(void*)))){
        return 0;
    }
    void *matrix1;
    if(!(matrix1 = malloc(3*sizeof(void*)))){
        return 0;
    }
    void **memArray2;
    if(!(memArray2 = (void**)malloc(3*sizeof(void*)))){
        return 0;
    }
    void *matrix2;
    if(!(matrix2 = malloc(3*sizeof(void*)))){
        return 0;
    }
    void **memArray3;
    if(!(memArray3 = (void**)malloc(3*sizeof(void*)))){
        return 0;
    }
    void *matrix3;
    if(!(matrix3 = malloc(3*sizeof(void*)))){
        return 0;
    }
    void* matrixArr[] = {matrix1, matrix2, matrix3};
    void** memArrayArr[] = {memArray1, memArray2, memArray3};
    const Algebra *rAlg = getRealNumberAlgebra();
    const Algebra *cAlg = getComplexNumberAlgebra();
    void *ptr1 = NULL;
    void *ptr2 = NULL;
    void *ptr3 = NULL;
    void* ptrArr[] = {ptr1, ptr2, ptr3};
    counter(ScalTest1(ptr1, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest2(ptr1, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest3(ptr1, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest4(ptr1, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest5(ptr1, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest6(ptr1, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest7(ptr1, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest8(ptr1, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest9(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest10(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest11(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest12(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest13(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest14(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest15(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest16(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest17(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest18(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest19(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest20(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest21(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest22(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest23(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest24(ptrArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest25(ptr1, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest26(ptr1, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest27(ptr1, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest28(ptr1, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest29(ptr1, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest30(ptr1, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest31(ptr1, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest32(ptr1, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest33(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest34(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest35(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest36(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest37(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest38(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest39(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest40(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest41(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest42(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest43(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest44(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest45(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest46(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest47(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(ScalTest48(ptrArr, cAlg), &passed, &badAlloc, &testsCnt);
    counter(Test1(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test2(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test3(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test4(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test5(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test6(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test7(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test8(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test9(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test10(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test11(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test12(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test13(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test14(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test15(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test16(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test17(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test18(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test19(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test20(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test21(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test22(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test23(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    counter(Test24(matrixArr, memArrayArr, rAlg), &passed, &badAlloc, &testsCnt);
    printf("%d of 72 tests passed successfully, %d of not passed tests failed because of bad memory allocation\n",
        passed, badAlloc);
    free(matrix1); free(matrix2); free(matrix3);
    free(memArray1); free(memArray2); free(memArray3);
    return 1;
}