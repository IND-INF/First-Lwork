#include <stdio.h>
#include <stdlib.h>
#include "tridiagonal_matrix.h"
#include "real_number_algebra.h"
#include "complex_number_algebra.h"

#define MACRO_FOR_CHECK_R_NUM_OPERATION(opFunction, checkRNumOpName) \
int checkRNumOpName(void **ptrArr, double *scalArr, const Algebra *alg){ \
\
    if(!(allocPtrArr(ptrArr, alg))) return 0; \
    assignRealNumber(ptrArr[0], scalArr[0]); \
    assignRealNumber(ptrArr[1], scalArr[1]); \
\
    opFunction(ptrArr[0], ptrArr[1], ptrArr[2]); \
\
    double ans; \
    recieveRealNumber(ptrArr[2], &ans); \
    freePtrArr(ptrArr); \
\
    if(ans != scalArr[2]){ \
        return 2; \
    } \
    return 1; \
\
}

#define MACRO_FOR_CHECK_C_NUM_OPERATION(opFunction, checkCNumOpName) \
int checkCNumOpName(void **ptrArr, double *scalArr, const Algebra *alg){ \
\
    if(!(allocPtrArr(ptrArr, alg))) return 0; \
    assignComplexNumber(ptrArr[0], scalArr[0], scalArr[1]); \
    assignComplexNumber(ptrArr[1], scalArr[2], scalArr[3]); \
\
    opFunction(ptrArr[0], ptrArr[1], ptrArr[2]); \
\
    double ansR, ansI; \
    recieveComplexNumber(ptrArr[2], &ansR, &ansI); \
    freePtrArr(ptrArr); \
\
    if(ansR != scalArr[4] || ansI != scalArr[5]){ \
        return 2; \
    } \
    return 1; \
\
}

typedef int (*makeFunction)(double*, void*, size_t, void**, const Algebra*);
typedef void (*makeNumFunction)(double*, void*);
typedef int (*compareFunction)(double*, void*, size_t, const Algebra*);

int checkRSize(const Algebra *alg){
    if(alg->elementSize != sizeof(double)) return 2;
    return 1;
}
int checkRNumAssigning(double scal, const Algebra *alg){

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return 0;

    assignRealNumber(ptrToElement, scal);
    if(*((double*)ptrToElement) != scal){
        free(ptrToElement);
        return 2;
    }

    assignRealNumber(ptrToElement, scal);
    if(*((double*)ptrToElement) != scal){
        free(ptrToElement);
        return 2;
    }

    free(ptrToElement);
    return 1;

}
int checkRNumRecieving(double scal, const Algebra *alg){

    void *ptrToElement;
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

int checkCSize(const Algebra *alg){
    if(alg->elementSize != 2*sizeof(double)) return 2;
    return 1;
}
int checkCNumAssigning(double rPart, double iPart, const Algebra *alg){

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return 0;

    assignComplexNumber(ptrToElement, rPart, iPart);
    if(*((double*)ptrToElement) != rPart || *((double*)ptrToElement+1) != iPart){
        free(ptrToElement);
        return 2;
    }

    assignComplexNumber(ptrToElement, rPart, iPart);
    if(*((double*)ptrToElement) != rPart || *((double*)ptrToElement+1) != iPart){
        free(ptrToElement);
        return 2;
    }

    free(ptrToElement);
    return 1;

}
int checkCNumRecieving(double rPart, double iPart, const Algebra *alg){

    void *ptrToElement;
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

MACRO_FOR_CHECK_R_NUM_OPERATION(alg->add, checkRNumAddition)
MACRO_FOR_CHECK_R_NUM_OPERATION(alg->multiply, checkRNumMult)

MACRO_FOR_CHECK_C_NUM_OPERATION(alg->add, checkCNumAddition)
MACRO_FOR_CHECK_C_NUM_OPERATION(alg->multiply, checkCNumMult)

int makeRTridiagonal(double *array, TriMatrix *matrix, size_t size, const Algebra *alg){

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return 0;

    if(!(createTridiagonalMatrix(matrix, size, alg))){
        free(ptrToElement);
        return 0;
    }
    for(size_t row = 0; row < size - 1; row++){
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

int makeThreeMatrices(double **arrayArr, TriMatrix **matrixArr, size_t size, const Algebra *alg){
    if(!(makeRTridiagonal(arrayArr[0], matrixArr[0], size, alg))){
        return 0;
    }
    if(!(makeRTridiagonal(arrayArr[1], matrixArr[1], size, alg))){
        freeTridiagonalMatrix(matrixArr[0]);
        return 0;
    }
    if(!(createTridiagonalMatrix(matrixArr[2], size, alg))){
        freeTridiagonalMatrix(matrixArr[0]);
        freeTridiagonalMatrix(matrixArr[1]);
        return 0;
    }
    return 1;
}

int compareRTridiagonal(double *array, TriMatrix *matrix, size_t size, const Algebra *alg){

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return 0;
    double number = 0;

    for(size_t row = 0; row < size - 1; row++){
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

int checkAdding(double **arrayArr, TriMatrix **matrixArr, size_t size, const Algebra *alg){

    if(!(makeThreeMatrices(arrayArr, matrixArr, size, alg))){
        return 0;
    }

    addTridiagonalMatrices(matrixArr[0], matrixArr[1], matrixArr[2]);

    int output = compareRTridiagonal(arrayArr[2], matrixArr[2], size, alg);

    freeTridiagonalMatrix(matrixArr[0]);
    freeTridiagonalMatrix(matrixArr[1]);
    freeTridiagonalMatrix(matrixArr[2]);
    return output;

}

int checkScalMult(double **arrayArr, TriMatrix **matrixArr, size_t size, const Algebra *alg){

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        return 0;
    }
    assignRealNumber(ptrToElement, arrayArr[0][0]);

    if(!(makeRTridiagonal(arrayArr[1], matrixArr[1], size, alg))){
        free(ptrToElement);
        return 0;
    }
    if(!(createTridiagonalMatrix(matrixArr[2], size, alg))){
        free(ptrToElement);
        freeTridiagonalMatrix(matrixArr[1]);
        return 0;
    }

    scalarlyMultiplyTridiagonalMatrix(ptrToElement, matrixArr[1], matrixArr[2], alg);

    int output = compareRTridiagonal(arrayArr[2], matrixArr[2], size, alg);

    free(ptrToElement);
    freeTridiagonalMatrix(matrixArr[1]);
    freeTridiagonalMatrix(matrixArr[2]);
    return output;

}

//Real numbers size:
int Test1(const Algebra *alg){
    return checkRSize(alg);
}
//Real numbers assigning:
int Test2(const Algebra *alg){
    double scal = 1;
    return checkRNumAssigning(scal, alg);
}
int Test3(const Algebra *alg){
    double scal = 0;
    return checkRNumAssigning(scal, alg);
}
int Test4(const Algebra *alg){
    double scal = -1;
    return checkRNumAssigning(scal, alg);
}
int Test5(const Algebra *alg){
    double scal = 1.5;
    return checkRNumAssigning(scal, alg);
}
int Test6(const Algebra *alg){
    double scal = -1.5;
    return checkRNumAssigning(scal, alg);
}
int Test7(const Algebra *alg){
    double scal = 0.25;
    return checkRNumAssigning(scal, alg);
}
int Test8(const Algebra *alg){
    double scal = -15;
    return checkRNumAssigning(scal, alg);
}
int Test9(const Algebra *alg){
    double scal = 1000;
    return checkRNumAssigning(scal, alg);
}
//Real numbers recieving:
int Test10(const Algebra *alg){
    double scal = 1;
    return checkRNumRecieving(scal, alg);
}
int Test11(const Algebra *alg){
    double scal = 0;
    return checkRNumRecieving(scal, alg);
}
int Test12(const Algebra *alg){
    double scal = -1;
    return checkRNumRecieving(scal, alg);
}
int Test13(const Algebra *alg){
    double scal = 1.5;
    return checkRNumRecieving(scal, alg);
}
int Test14(const Algebra *alg){
    double scal = -1.5;
    return checkRNumRecieving(scal, alg);
}
int Test15(const Algebra *alg){
    double scal = 0.25;
    return checkRNumRecieving(scal, alg);
}
int Test16(const Algebra *alg){
    double scal = -15;
    return checkRNumRecieving(scal, alg);
}
int Test17(const Algebra *alg){
    double scal = 1000;
    return checkRNumRecieving(scal, alg);
}
//Real numbers adding:
int Test18(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        3,
        2,
        5
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int Test19(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,
        0,
        0
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int Test20(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -1,
        -6,
        -7
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int Test21(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        3,
        -5,
        -2
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int Test22(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1.5,
        2,
        3.5
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int Test23(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        2.5,
        -1.5,
        1
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int Test24(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        2,
        -2,
        0
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
int Test25(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1.25,
        0.5,
        1.75
    };
    return checkRNumAddition(ptrArr, scalArr, alg);
}
//Real numbers multiplying:
int Test26(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        3,
        2,
        6
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int Test27(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1,
        5,
        5
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int Test28(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,
        0,
        0
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int Test29(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        2,
        -1,
        -2
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int Test30(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0.5,
        5,
        2.5
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int Test31(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -4,
        1.5,
        -6
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int Test32(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,
        100,
        0
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}
int Test33(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0.5,
        4.5,
        2.25
    };
    return checkRNumMult(ptrArr, scalArr, alg);
}

//Complex numbers size:
int Test34(const Algebra *alg){
    return checkCSize(alg);
}
//Complex numbers assigning
int Test35(const Algebra *alg){
    double rPart = 1, iPart = 1;
    return checkCNumAssigning(rPart, iPart, alg);
}
int Test36(const Algebra *alg){
    double rPart = 0, iPart = 0;
    return checkCNumAssigning(rPart, iPart, alg);
}
int Test37(const Algebra *alg){
    double rPart = 2, iPart = 0;
    return checkCNumAssigning(rPart, iPart, alg);
}
int Test38(const Algebra *alg){
    double rPart = 0, iPart = -3;
    return checkCNumAssigning(rPart, iPart, alg);
}
int Test39(const Algebra *alg){
    double rPart = -4, iPart = 5;
    return checkCNumAssigning(rPart, iPart, alg);
}
int Test40(const Algebra *alg){
    double rPart = 0.5, iPart = 1.5;
    return checkCNumAssigning(rPart, iPart, alg);
}
int Test41(const Algebra *alg){
    double rPart = -2.5, iPart = 100;
    return checkCNumAssigning(rPart, iPart, alg);
}
int Test42(const Algebra *alg){
    double rPart = 0.25, iPart = -3.5;
    return checkCNumAssigning(rPart, iPart, alg);
}
//Complex numbers recieving:
int Test43(const Algebra *alg){
    double rPart = 1, iPart = 1;
    return checkCNumRecieving(rPart, iPart, alg);
}
int Test44(const Algebra *alg){
    double rPart = 0, iPart = 0;
    return checkCNumRecieving(rPart, iPart, alg);
}
int Test45(const Algebra *alg){
    double rPart = 2, iPart = 0;
    return checkCNumRecieving(rPart, iPart, alg);
}
int Test46(const Algebra *alg){
    double rPart = 0, iPart = -3;
    return checkCNumRecieving(rPart, iPart, alg);
}
int Test47(const Algebra *alg){
    double rPart = -4, iPart = 5;
    return checkCNumRecieving(rPart, iPart, alg);
}
int Test48(const Algebra *alg){
    double rPart = 0.5, iPart = 1.5;
    return checkCNumRecieving(rPart, iPart, alg);
}
int Test49(const Algebra *alg){
    double rPart = -2.5, iPart = 100;
    return checkCNumRecieving(rPart, iPart, alg);
}
int Test50(const Algebra *alg){
    double rPart = 0.25, iPart = -3.5;
    return checkCNumRecieving(rPart, iPart, alg);
}
//Complex numbers adding:
int Test51(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        3,4,
        5,6,
        8,10
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int Test52(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,0,
        2,3,
        2,3
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int Test53(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        5,6,
        -2,-1,
        3,5
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int Test54(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1,0.5,
        1.25,0.25,
        2.25,0.75
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int Test55(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,0,
        0,0,
        0,0
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int Test56(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -5,-6,
        -7,-8,
        -12,-14
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int Test57(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -5.5,6.5,
        7.5,-8.5,
        2,-2
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
int Test58(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        5.5,6.5,
        -7.25,-8.25,
        -1.75,-1.75
    };
    return checkCNumAddition(ptrArr, scalArr, alg);
}
//Complex numbers multiplying
int Test59(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        1,0,
        1,0,
        1,0
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int Test60(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,1,
        0,1,
        -1,0
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int Test61(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0,0,
        0,0,
        0,0
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int Test62(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        2,3,
        4,5,
        -7,22
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int Test63(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        5,6,
        0,0,
        0,0
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int Test64(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        -1,2,
        3,-4,
        5,10
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int Test65(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0.5,1.5,
        2.5,3.5,
        -4,5.5
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}
int Test66(void **ptrArr, const Algebra *alg){
    double scalArr[] = {
        0.25,1,
        -1,2,
        -2.25,-0.5
    };
    return checkCNumMult(ptrArr, scalArr, alg);
}

//Matrices adding:
int Test67(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 1;
    double fstMat[] = {5};
    double sndMat[] = {3};
    double resMat[] = {8};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, size, alg);
}
int Test68(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 2;
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
    return checkAdding(arrayArr, matrixArr, size, alg);
}
int Test69(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 2;
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
    return checkAdding(arrayArr, matrixArr, size, alg);
}
int Test70(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 3;
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
    return checkAdding(arrayArr, matrixArr, size, alg);
}
int Test71(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 3;
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
    return checkAdding(arrayArr, matrixArr, size, alg);
}
int Test72(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 4;
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
    return checkAdding(arrayArr, matrixArr, size, alg);
}
int Test73(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 5;
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
    return checkAdding(arrayArr, matrixArr, size, alg);
}
int Test74(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 1;
    double fstMat[] = {0};
    double sndMat[] = {-1};
    double resMat[] = {-1};
    double* arrayArr[] = {fstMat, sndMat, resMat};
    return checkAdding(arrayArr, matrixArr, size, alg);
}
//Matrices multiplying:
int Test75(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 1;
    double scal[] = {2};
    double mat[] = {5};
    double resMat[] = {10};
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, size, alg);
}
int Test76(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 2;
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
    return checkScalMult(arrayArr, matrixArr, size, alg);
}
int Test77(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 3;
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
    return checkScalMult(arrayArr, matrixArr, size, alg);
}
int Test78(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 4;
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
    return checkScalMult(arrayArr, matrixArr, size, alg);
}
int Test79(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 5;
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
    return checkScalMult(arrayArr, matrixArr, size, alg);
}
int Test80(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 2;
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
    return checkScalMult(arrayArr, matrixArr, size, alg);
}
int Test81(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 1;
    double scal[] = {-2};
    double mat[] = {-3};
    double resMat[] = {6};
    double* arrayArr[] = {scal, mat, resMat};
    return checkScalMult(arrayArr, matrixArr, size, alg);
}
int Test82(TriMatrix **matrixArr, const Algebra *alg){
    size_t size = 3;
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
    return checkScalMult(arrayArr, matrixArr, size, alg);
}

void counter(int testReturn, int *passed, int *testsCnt){

    if(testReturn == 2){
        printf("%d failed WA\n", *testsCnt);
    }else if(testReturn == 0){
        printf("%d failed BadAlloc\n", *testsCnt);
    }else (*passed)++;

    (*testsCnt)++;

}

int doTheTest(void){

    int testsCnt = 1;
    int passed = 0;

    TriMatrix *matrix1;
    if(!declareTridiagonalMatrix(&matrix1)){
        return 0;
    }
    TriMatrix *matrix2;
    if(!declareTridiagonalMatrix(&matrix2)){
        return 0;
    }
    TriMatrix *matrix3;
    if(!declareTridiagonalMatrix(&matrix3)){
        return 0;
    }
    TriMatrix* matrixArr[] = {matrix1, matrix2, matrix3};

    const Algebra *rAlg = getRealNumberAlgebra();
    const Algebra *cAlg = getComplexNumberAlgebra();

    void *ptr1 = NULL;
    void *ptr2 = NULL;
    void *ptr3 = NULL;
    void* ptrArr[] = {ptr1, ptr2, ptr3};

    counter(Test1(rAlg), &passed, &testsCnt);

    counter(Test2(rAlg), &passed, &testsCnt);
    counter(Test3(rAlg), &passed, &testsCnt);
    counter(Test4(rAlg), &passed, &testsCnt);
    counter(Test5(rAlg), &passed, &testsCnt);
    counter(Test6(rAlg), &passed, &testsCnt);
    counter(Test7(rAlg), &passed, &testsCnt);
    counter(Test8(rAlg), &passed, &testsCnt);
    counter(Test9(rAlg), &passed, &testsCnt);

    counter(Test10(rAlg), &passed, &testsCnt);
    counter(Test11(rAlg), &passed, &testsCnt);
    counter(Test12(rAlg), &passed, &testsCnt);
    counter(Test13(rAlg), &passed, &testsCnt);
    counter(Test14(rAlg), &passed, &testsCnt);
    counter(Test15(rAlg), &passed, &testsCnt);
    counter(Test16(rAlg), &passed, &testsCnt);
    counter(Test17(rAlg), &passed, &testsCnt);

    counter(Test18(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test19(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test20(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test21(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test22(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test23(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test24(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test25(ptrArr, rAlg), &passed, &testsCnt);

    counter(Test26(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test27(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test28(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test29(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test30(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test31(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test32(ptrArr, rAlg), &passed, &testsCnt);
    counter(Test33(ptrArr, rAlg), &passed, &testsCnt);

    counter(Test34(cAlg), &passed, &testsCnt);
    
    counter(Test35(cAlg), &passed, &testsCnt);
    counter(Test36(cAlg), &passed, &testsCnt);
    counter(Test37(cAlg), &passed, &testsCnt);
    counter(Test38(cAlg), &passed, &testsCnt);
    counter(Test39(cAlg), &passed, &testsCnt);
    counter(Test40(cAlg), &passed, &testsCnt);
    counter(Test41(cAlg), &passed, &testsCnt);
    counter(Test42(cAlg), &passed, &testsCnt);

    counter(Test43(cAlg), &passed, &testsCnt);
    counter(Test44(cAlg), &passed, &testsCnt);
    counter(Test45(cAlg), &passed, &testsCnt);
    counter(Test45(cAlg), &passed, &testsCnt);
    counter(Test47(cAlg), &passed, &testsCnt);
    counter(Test48(cAlg), &passed, &testsCnt);
    counter(Test49(cAlg), &passed, &testsCnt);
    counter(Test50(cAlg), &passed, &testsCnt);
    
    counter(Test51(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test52(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test53(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test54(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test55(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test56(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test57(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test58(ptrArr, cAlg), &passed, &testsCnt);

    counter(Test59(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test60(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test61(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test62(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test63(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test64(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test65(ptrArr, cAlg), &passed, &testsCnt);
    counter(Test66(ptrArr, cAlg), &passed, &testsCnt);

    counter(Test67(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test68(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test69(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test70(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test71(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test72(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test73(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test74(matrixArr, rAlg), &passed, &testsCnt);

    counter(Test75(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test76(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test77(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test78(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test79(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test80(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test81(matrixArr, rAlg), &passed, &testsCnt);
    counter(Test82(matrixArr, rAlg), &passed, &testsCnt);

    printf("%d of 82 tests passed successfully\n", passed);

    free(matrix1); free(matrix2); free(matrix3);
    return 1;

}