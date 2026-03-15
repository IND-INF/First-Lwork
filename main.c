#include <stdio.h>
#include <stdlib.h>
#include "tridiagonal_matrix.h"
#include "integer_algebra.h"
#include "real_number_algebra.h"
#include "complex_number_algebra.h"
#include "test_of_the_structure.h"

typedef void (*ScanModification)(void*, unsigned int, unsigned int, void*, const Algebra*);
typedef void (*ScanAssignment)(void *);

void printComplexNumbersNotification(void){
    printf("(Enter by pairs of real numbers where the first number is the real part and the second number is the imaginary part without i)\n");
}

void freeMatrix(void *matrix, void **memArray){
    free(*(memArray));
    free(*(memArray+1));
    free(*(memArray+2));
    free(matrix);
}

int allocateMatrix(void **matrix, void **memArray, unsigned int matrixSize,
    const Algebra *matrixTypeAlgebra){
    if(!(allocateMemArray(memArray, matrixSize, matrixTypeAlgebra))){
        return 0;
    }
    if(!(*matrix = malloc(3*sizeof(void*)))){
        free(*(memArray));
        free(*(memArray+1));
        free(*(memArray+2));
        return 0;
    }
    createTridiagonalMatrix(memArray, *matrix);
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
    const Algebra *matrixTypeAlgebra){
    scanAndAssignInt(pointer);
    modifyTridiagonalMatrixElement(row, column, matrix, pointer, matrixTypeAlgebra);
}

void scanAndModifyR(void *pointer, unsigned int row, unsigned int column, void *matrix,
    const Algebra *matrixTypeAlgebra){
    scanAndAssignR(pointer);
    modifyTridiagonalMatrixElement(row, column, matrix, pointer, matrixTypeAlgebra);
}

void scanAndModifyC(void *pointer, unsigned int row, unsigned int column, void *matrix,
    const Algebra *matrixTypeAlgebra){
    scanAndAssignC(pointer);
    modifyTridiagonalMatrixElement(row, column, matrix, pointer, matrixTypeAlgebra);
}

int main(int argc, char* argv[]){
    doTheTest();
    int exitFlag = 0;
    unsigned int enteredValue = 0;
    unsigned int matrixSize = 0;
    unsigned int matrixType = 0;
    unsigned int matrixPreviousSize = 0;
    unsigned int matrixPreviousType = 0;
    unsigned int unssub1 = 0, unssub2 = 0;
    void *ptrsub = NULL;
    const Algebra *matrixTypeAlgebra = NULL;
    const Algebra *newMatrixTypeAlgebra = NULL;
    ScanModification scanAndModify;
    ScanAssignment scanAndAssign;
    void **memArray;
    if(!(memArray = malloc(3*sizeof(void*)))){
        printf("Bad allocation\n");
        return 0;
    }
    void *matrix = NULL;
    void **memArraySub;
    if(!(memArraySub = malloc(3*sizeof(void*)))){
        printf("Bad allocation\n");
        return 0;
    }
    void *matrixSub = NULL;
    void **memArrayInterm;
    if(!(memArrayInterm = malloc(3*sizeof(void*)))){
        printf("Bad allocation\n");
        return 0;
    }
    void *matrixInterm = NULL;
    while(!exitFlag){
        printf("For assistance, enter 0\n");
        scanf("%d", &enteredValue);
        switch(enteredValue){
            case 0:
                printf("For the creation of a matrix, enter 1\n");
                printf("For the modification of a matrix element, enter 2\n");
                printf("For the addition of matrices, enter 3\n");
                printf("For the multiplication of matrices, enter 4\n");
                printf("For the scalar multiplication of a matrix, enter 5\n");
                printf("For the reciept of a matrix element, enter 6\n");
                printf("For the reciept of a matrix, enter 7\n");
                printf("For exit, enter 8\n");
                break;
            case 1:
                matrixPreviousSize = matrixSize;
                printf("Enter the size of the matrix\n");
                scanf("%d", &matrixSize);
                if(matrixSize < 1){
                    printf("Too little number\n");
                    matrixSize = matrixPreviousSize;
                    break;
                }
                printf("Enter the type of the matrix:\n");
                printf("- For the integer type, enter 1;\n");
                printf("- For the real number type, enter 2;\n");
                printf("- For the complex number type, enter 3\n");
                matrixPreviousType = matrixType;
                scanf("%d", &matrixType);
                if(matrixType != 1 && matrixType != 2 && matrixType != 3){
                    printf("Wrong number\n");
                    matrixType = matrixPreviousType;
                    matrixSize = matrixPreviousSize;
                    break;
                }
                if(matrixType == 1){
                    if(!(newMatrixTypeAlgebra = getIntegerAlgebra())){
                        printf("Bad algebra allocation\n");
                        matrixType = matrixPreviousType;
                        matrixSize = matrixPreviousSize;
                        break;
                    }
                    scanAndAssign = scanAndAssignInt;
                    scanAndModify = scanAndModifyInt;
                }else if(matrixType == 2){
                    if(!(newMatrixTypeAlgebra = getRealNumberAlgebra())){
                        printf("Bad algebra allocation\n");
                        matrixType = matrixPreviousType;
                        matrixSize = matrixPreviousSize;
                        break;
                    }
                    scanAndAssign = scanAndAssignR;
                    scanAndModify = scanAndModifyR;
                }else{
                    if(!(newMatrixTypeAlgebra = getComplexNumberAlgebra())){
                        printf("Bad algebra allocation\n");
                        matrixType = matrixPreviousType;
                        matrixSize = matrixPreviousSize;
                        break;
                    }
                    scanAndAssign = scanAndAssignC;
                    scanAndModify = scanAndModifyC;
                }
                matrixTypeAlgebra = newMatrixTypeAlgebra;
                if(matrix){
                    freeMatrix(matrix, memArray);
                }
                if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                    printf("Bad allocation\n");
                    break;
                }
                if(!(allocateMatrix(&matrix, memArray, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    free(ptrsub);
                    break;
                }
                if(matrixType == 3){
                    printComplexNumbersNotification();
                }
                printf("Enter the superior diagonal of the matrix\n");
                for(unsigned int number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrsub, number, number+1, matrix, matrixTypeAlgebra);
                }
                printf("Enter the principal diagonal of the matrix\n");
                for(unsigned int number = 0; number < matrixSize; number++){
                    scanAndModify(ptrsub, number, number, matrix, matrixTypeAlgebra);
                }
                printf("Enter the inferior diagonal of the matrix\n");
                for(unsigned int number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrsub, number+1, number, matrix, matrixTypeAlgebra);
                }
                free(ptrsub);
                break;
            case 2:
                if(!matrix){
                    printf("The matrix does not exist\n");
                    break;
                }
                printf("Enter the row and the column of the element (starting the count from 0)\n");
                scanf("%d", &unssub1);
                scanf("%d", &unssub2);
                if(unssub1 >= matrixSize || unssub2 >= matrixSize){
                    printf("The position does not exist\n");
                    break;
                }
                if((int)(unssub1 - unssub2) > 1 || (int)(unssub1 - unssub2) < -1){
                    printf("The position is out of the three diagonals\n");
                    break;
                }
                if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                    printf("Bad allocation\n");
                    break;
                }
                printf("Enter the new value\n");
                if(matrixType == 3){
                    printComplexNumbersNotification();
                }
                scanAndModify(ptrsub, unssub1, unssub2, matrix, matrixTypeAlgebra);
                free(ptrsub);
                break;
            case 3:
                if(!matrix){
                    printf("The matrix does not exist\n");
                    break;
                }
                if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                    printf("Bad allocation\n");
                    break;
                }
                if(!(allocateMatrix(&matrixSub, memArraySub, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    free(ptrsub);
                    break;
                }
                if(!(allocateMatrix(&matrixInterm, memArrayInterm, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    freeMatrix(matrixSub, memArraySub);
                    free(ptrsub);
                    break;
                }
                printf("Notification: the matrix size is %d, the matrix type is ", matrixSize);
                if(matrixType == 1){
                    printf("the integer type\n");
                }else if(matrixType == 2){
                    printf("the real number type\n");
                }else{
                    printf("the complex number type\n");
                    printComplexNumbersNotification();
                }
                printf("Enter the superior diagonal of the matrix you want the current matrix to be added to\n");
                for(unsigned int number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrsub, number, number+1, matrixSub, matrixTypeAlgebra);
                }
                printf("Enter the principal diagonal of the matrix you want the current matrix to be added to\n");
                for(unsigned int number = 0; number < matrixSize; number++){
                    scanAndModify(ptrsub, number, number, matrixSub, matrixTypeAlgebra);
                }
                printf("Enter the inferior diagonal of the matrix you want the current matrix to be added to\n");
                for(unsigned int number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrsub, number+1, number, matrixSub, matrixTypeAlgebra);
                }
                addTridiagonalMatrices(matrixSize, matrix, matrixSub, matrixInterm, matrixTypeAlgebra);
                freeMatrix(matrixSub, memArraySub);
                free(ptrsub);
                freeMatrix(matrix, memArray);
                matrix = matrixInterm;
                matrixInterm = NULL;
                ptrsub = (void*)memArray;
                memArray = memArrayInterm;
                memArrayInterm = (void**)ptrsub;
                ptrsub = NULL;
                break;
            case 4:
                if(!matrix){
                    printf("The matrix does not exist\n");
                    break;
                }
                if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                    printf("Bad allocation\n");
                    break;
                }
                if(!(allocateMatrix(&matrixSub, memArraySub, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    free(ptrsub);
                    break;
                }
                if(!(allocateMatrix(&matrixInterm, memArrayInterm, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    freeMatrix(matrixSub, memArraySub);
                    free(ptrsub);
                    break;
                }
                printf("Notification: the matrix size is %d, the matrix type is ", matrixSize);
                if(matrixType == 1){
                    printf("the integer type\n");
                }else if(matrixType == 2){
                    printf("the real number type\n");
                }else{
                    printf("the complex number type\n");
                    printComplexNumbersNotification();
                }
                printf("Enter the superior diagonal of the matrix you want the current matrix to be multiplied by\n");
                for(unsigned int number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrsub, number, number+1, matrixSub, matrixTypeAlgebra);
                }
                printf("Enter the principal diagonal of the matrix you want the current matrix to be multiplied by\n");
                for(unsigned int number = 0; number < matrixSize; number++){
                    scanAndModify(ptrsub, number, number, matrixSub, matrixTypeAlgebra);
                }
                printf("Enter the inferior diagonal of the matrix you want the current matrix to be multiplied by\n");
                for(unsigned int number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrsub, number+1, number, matrixSub, matrixTypeAlgebra);
                }
                if(!(multiplyTridiagonalMatrices(matrixSize, matrix, matrixSub, matrixInterm,
                    matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    freeMatrix(matrixInterm, memArrayInterm);
                    freeMatrix(matrixSub, memArraySub);
                    free(ptrsub);
                    break;
                }
                freeMatrix(matrixSub, memArraySub);
                free(ptrsub);
                freeMatrix(matrix, memArray);
                matrix = matrixInterm;
                matrixInterm = NULL;
                ptrsub = (void*)memArray;
                memArray = memArrayInterm;
                memArrayInterm = (void**)ptrsub;
                ptrsub = NULL;
                break;
            case 5:
                if(!matrix){
                    printf("The matrix does not exist\n");
                    break;
                }
                if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                    printf("Bad allocation\n");
                    break;
                }
                if(!(allocateMatrix(&matrixInterm, memArrayInterm, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    free(ptrsub);
                    break;
                }
                printf("Notification: the matrix type is ");
                if(matrixType == 1){
                    printf("the integer type\n");
                }else if(matrixType == 2){
                    printf("the real number type\n");
                }else{
                    printf("the complex number type\n");
                    printComplexNumbersNotification();
                }
                printf("Enter the scalar you want to multiply by the current matrix\n");
                scanAndAssign(ptrsub);
                scalarlyMultiplyTridiagonalMatrix(matrixSize, ptrsub, matrix, matrixInterm, matrixTypeAlgebra);
                free(ptrsub);
                freeMatrix(matrix, memArray);
                matrix = matrixInterm;
                matrixInterm = NULL;
                ptrsub = (void*)memArray;
                memArray = memArrayInterm;
                memArrayInterm = (void**)ptrsub;
                ptrsub = NULL;
                break;
            case 6:
                if(!matrix){
                    printf("The matrix does not exist\n");
                    break;
                }
                printf("Enter the row and the column of the element (starting the count from 0)\n");
                scanf("%d", &unssub1);
                scanf("%d", &unssub2);
                if(unssub1 >= matrixSize || unssub2 >= matrixSize){
                    printf("The position does not exist\n");
                    break;
                }
                printTridiagonalMatrixElement(unssub1, unssub2, matrix, matrixTypeAlgebra);
                printf("\n");
                break;
            case 7:
                if(!matrix){
                    printf("The matrix does not exist\n");
                    break;
                }
                for(unsigned int row = 0, column; row < matrixSize; row++){
                    for(column = 0; column < matrixSize - 1; column++){
                        printTridiagonalMatrixElement(row, column, matrix, matrixTypeAlgebra);
                        printf(" ");
                    }
                    printTridiagonalMatrixElement(row, column, matrix, matrixTypeAlgebra);
                    printf("\n");
                }
                break;
            default:
                exitFlag = 1;
        }
    }
    if(matrix) freeMatrix(matrix, memArray);
    free(memArray);
    free(memArraySub);
    free(memArrayInterm);
    return 0;
}