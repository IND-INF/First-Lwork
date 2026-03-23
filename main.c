#include <stdio.h>
#include <stdlib.h>
#include "tridiagonal_matrix.h"
#include "real_number_algebra.h"
#include "complex_number_algebra.h"
#include "test_of_the_structure.h"

typedef void (*ScanModification)(void*, size_t, size_t, TriMatrix*, const Algebra*);
typedef void (*ScanAssignment)(void *);

#define MACRO_FOR_SCAN_AND_MODIFY(scanFunction, scanName) \
void scanName(void *pointer, size_t row, size_t column, TriMatrix *matrix, const Algebra *matrixTypeAlgebra){ \
    scanFunction(pointer); \
    modifyTridiagonalMatrixElement(row, column, matrix, pointer, matrixTypeAlgebra); \
}

#define WRONG_INPUT_MACRO \
scanf("%c", &refuse); \
printWrongInput();

void printComplexNumbersNotification(void){
    printf("(Enter by pairs of real numbers where the first number is the real part and the second number is the imaginary part without i)\n");
}

void printWrongInput(void){
    printf("Wrong input, enter a number with the Arabic numerals\n");
}

void scanAndAssignR(void *pointer){

    double subservient;
    char refuse;

    while(!(scanf("%lf", &subservient))){
        WRONG_INPUT_MACRO
    }

    assignRealNumber(pointer, subservient);
    
}

void scanAndAssignC(void *pointer){

    double subservient1, subservient2;
    char refuse;

    while(!(scanf("%lf", &subservient1))){
        WRONG_INPUT_MACRO
    }
    while(!(scanf("%lf", &subservient2))){
        WRONG_INPUT_MACRO
    }

    assignComplexNumber(pointer, subservient1, subservient2);

}

MACRO_FOR_SCAN_AND_MODIFY(scanAndAssignR, scanAndModifyR)

MACRO_FOR_SCAN_AND_MODIFY(scanAndAssignC, scanAndModifyC)

int main(void){

    doTheTest();

    char refuse;
    int exitFlag = 0;
    size_t enteredValue = 0;

    size_t matrixSize = 0;
    size_t matrixType = 0;
    const Algebra *matrixTypeAlgebra = NULL;

    size_t newMatrixSize = 0;
    const Algebra *newMatrixTypeAlgebra = NULL;

    size_t sizeSub1 = 0, sizeSub2 = 0;
    void *ptrSub = NULL;

    ScanModification scanAndModify;
    ScanAssignment scanAndAssign;

    int matrixExistence = 0;
    TriMatrix *matrix;
    if(!declareTridiagonalMatrix(&matrix)){
        printf("Bad allocation\n");
        return 0;
    }
    TriMatrix *matrixSub;
    if(!declareTridiagonalMatrix(&matrixSub)){
        printf("Bad allocation\n");
        return 0;
    }
    TriMatrix *matrixInterm;
    if(!declareTridiagonalMatrix(&matrixInterm)){
        printf("Bad allocation\n");
        return 0;
    }

    while(!exitFlag){

        printf("For assistance, enter 0\n");
        while(!(scanf("%d", &enteredValue))){
            WRONG_INPUT_MACRO
        }

        switch(enteredValue){
            case 0:
                printf("For the creation of a matrix, enter 1\n");
                printf("For the modification of a matrix element, enter 2\n");
                printf("For the addition of matrices, enter 3\n");
                printf("For the scalar multiplication of a matrix, enter 4\n");
                printf("For the reciept of a matrix element, enter 5\n");
                printf("For the reciept of a matrix, enter 6\n");
                printf("For exit, enter 7\n");
                break;
            case 1:

                printf("Enter the size of the matrix\n");
                while(!(scanf("%d", &newMatrixSize))){
                    WRONG_INPUT_MACRO
                }
                if(newMatrixSize < 1){
                    printf("Too little number\n");
                    break;
                }

                printf("Enter the type of the matrix:\n");
                printf("- For the real number type, enter 1;\n");
                printf("- For the complex number type, enter 2\n");
                while(!(scanf("%d", &matrixType))){
                    WRONG_INPUT_MACRO
                }
                if(matrixType != 1 && matrixType != 2){
                    printf("Wrong number\n");
                    break;
                }

                if(matrixType == 1){
                    if(!(newMatrixTypeAlgebra = getRealNumberAlgebra())){
                        printf("Bad algebra allocation\n");
                        break;
                    }
                    scanAndAssign = scanAndAssignR;
                    scanAndModify = scanAndModifyR;
                }else{
                    if(!(newMatrixTypeAlgebra = getComplexNumberAlgebra())){
                        printf("Bad algebra allocation\n");
                        break;
                    }
                    scanAndAssign = scanAndAssignC;
                    scanAndModify = scanAndModifyC;
                }

                if(!(ptrSub = malloc(newMatrixTypeAlgebra->elementSize))){
                    printf("Bad allocation\n");
                    break;
                }
                
                matrixSize = newMatrixSize;
                matrixTypeAlgebra = newMatrixTypeAlgebra;

                if(matrixExistence){
                    freeTridiagonalMatrix(matrix);
                    matrixExistence = 0;
                }
                if(!(createTridiagonalMatrix(matrix, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    free(ptrSub);
                    break;
                }

                if(matrixType == 2){
                    printComplexNumbersNotification();
                }
                
                printf("Enter the superior diagonal of the matrix\n");
                for(size_t number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrSub, number, number+1, matrix, matrixTypeAlgebra);
                }
                printf("Enter the principal diagonal of the matrix\n");
                for(size_t number = 0; number < matrixSize; number++){
                    scanAndModify(ptrSub, number, number, matrix, matrixTypeAlgebra);
                }
                printf("Enter the inferior diagonal of the matrix\n");
                for(size_t number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrSub, number+1, number, matrix, matrixTypeAlgebra);
                }
                matrixExistence = 1;

                free(ptrSub);
                break;

            case 2:

                if(!matrixExistence){
                    printf("The matrix does not exist\n");
                    break;
                }

                printf("Enter the row and the column of the element (starting the count from 0)\n");
                while(!(scanf("%d", &sizeSub1))){
                    WRONG_INPUT_MACRO
                }
                while(!(scanf("%d", &sizeSub2))){
                    WRONG_INPUT_MACRO
                }
                if(sizeSub1 >= matrixSize || sizeSub2 >= matrixSize){
                    printf("The position does not exist\n");
                    break;
                }
                if((int)(sizeSub1 - sizeSub2) > 1 || (int)(sizeSub1 - sizeSub2) < -1){
                    printf("The position is out of the three diagonals\n");
                    break;
                }

                if(!(ptrSub = malloc(matrixTypeAlgebra->elementSize))){
                    printf("Bad allocation\n");
                    break;
                }

                printf("Notification: the matrix type is ");
                if(matrixType == 1){
                    printf("the real number type\n");
                }else{
                    printf("the complex number type\n");
                    printComplexNumbersNotification();
                }

                printf("Enter the new value\n");
                if(matrixType == 2){
                    printComplexNumbersNotification();
                }
                scanAndModify(ptrSub, sizeSub1, sizeSub2, matrix, matrixTypeAlgebra);

                free(ptrSub);
                break;

            case 3:

                if(!matrixExistence){
                    printf("The matrix does not exist\n");
                    break;
                }

                if(!(ptrSub = malloc(matrixTypeAlgebra->elementSize))){
                    printf("Bad allocation\n");
                    break;
                }
                if(!(createTridiagonalMatrix(matrixSub, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    free(ptrSub);
                    break;
                }
                if(!(createTridiagonalMatrix(matrixInterm, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    freeTridiagonalMatrix(matrix);
                    free(ptrSub);
                    break;
                }

                printf("Notification: the matrix size is %d, the matrix type is ", matrixSize);
                if(matrixType == 1){
                    printf("the real number type\n");
                }else{
                    printf("the complex number type\n");
                    printComplexNumbersNotification();
                }

                printf("Enter the superior diagonal of the matrix you want the current matrix to be added to\n");
                for(size_t number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrSub, number, number+1, matrixSub, matrixTypeAlgebra);
                }
                printf("Enter the principal diagonal of the matrix you want the current matrix to be added to\n");
                for(size_t number = 0; number < matrixSize; number++){
                    scanAndModify(ptrSub, number, number, matrixSub, matrixTypeAlgebra);
                }
                printf("Enter the inferior diagonal of the matrix you want the current matrix to be added to\n");
                for(size_t number = 0; number < matrixSize - 1; number++){
                    scanAndModify(ptrSub, number+1, number, matrixSub, matrixTypeAlgebra);
                }

                addTridiagonalMatrices(matrix, matrixSub, matrixInterm);

                freeTridiagonalMatrix(matrixSub);
                free(ptrSub);
                freeTridiagonalMatrix(matrix);

                ptrSub = (void*)matrix;
                matrix = matrixInterm;
                matrixInterm = (TriMatrix*)ptrSub;
                ptrSub = NULL;

                break;

            case 4:

                if(!matrixExistence){
                    printf("The matrix does not exist\n");
                    break;
                }

                if(!(ptrSub = malloc(matrixTypeAlgebra->elementSize))){
                    printf("Bad allocation\n");
                    break;
                }
                if(!(createTridiagonalMatrix(matrixInterm, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation\n");
                    free(ptrSub);
                    break;
                }

                printf("Notification: the matrix type is ");
                if(matrixType == 1){
                    printf("the real number type\n");
                }else{
                    printf("the complex number type\n");
                    printComplexNumbersNotification();
                }

                printf("Enter the scalar you want to multiply by the current matrix\n");
                scanAndAssign(ptrSub);

                scalarlyMultiplyTridiagonalMatrix(ptrSub, matrix, matrixInterm, matrixTypeAlgebra);

                free(ptrSub);
                freeTridiagonalMatrix(matrix);

                ptrSub = (void*)matrix;
                matrix = matrixInterm;
                matrixInterm = (TriMatrix*)ptrSub;
                ptrSub = NULL;

                break;

            case 5:

                if(!matrixExistence){
                    printf("The matrix does not exist\n");
                    break;
                }

                printf("Enter the row and the column of the element (starting the count from 0)\n");
                while(!(scanf("%d", &sizeSub1))){
                    WRONG_INPUT_MACRO
                }
                while(!(scanf("%d", &sizeSub2))){
                    WRONG_INPUT_MACRO
                }
                if(sizeSub1 >= matrixSize || sizeSub2 >= matrixSize){
                    printf("The position does not exist\n");
                    break;
                }

                printTridiagonalMatrixElement(sizeSub1, sizeSub2, matrix);
                printf("\n");

                break;

            case 6:
            
                if(!matrixExistence){
                    printf("The matrix does not exist\n");
                    break;
                }

                for(size_t row = 0, column; row < matrixSize; row++){
                    for(column = 0; column < matrixSize - 1; column++){
                        printTridiagonalMatrixElement(row, column, matrix);
                        printf(" ");
                    }
                    printTridiagonalMatrixElement(row, column, matrix);
                    printf("\n");
                }

                break;

            default:
                exitFlag = 1;
        }
    }

    free(matrix);
    free(matrixSub);
    free(matrixInterm);
    return 0;
}