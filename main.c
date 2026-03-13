#include "headers_and_functions.h"
#include "test_of_the_structure.h"

int main(int argc, char* argv[]){
    doTheTest();
    unsigned int enteredValue = 0;
    unsigned int matrixSize = 0;
    unsigned int matrixType = 0;
    unsigned int matrixPreviousSize = 0;
    unsigned int matrixPreviousType = 0;
    unsigned int unssub1 = 0, unssub2 = 0;
    void *ptrsub = NULL;
    Algebra *matrixTypeAlgebra = NULL;
    ScanModification scanAndModify;
    ScanAssignment scanAndAssign;
    void ***diagonalsArray;
    if(!(diagonalsArray = malloc(3*sizeof(void**)))){
        printf("Bad allocation\n");
        return 0;
    }
    void *matrix = NULL;
    void ***diagonalsArraySub;
    if(!(diagonalsArraySub = malloc(3*sizeof(void**)))){
        printf("Bad allocation\n");
        return 0;
    }
    void *matrixSub = NULL;
    void ***diagonalsArrayInterm;
    if(!(diagonalsArrayInterm = malloc(3*sizeof(void**)))){
        printf("Bad allocation\n");
        return 0;
    }
    void *matrixInterm = NULL;
    while(1){
        printf("For assistance, enter 0\n");
        scanf("%d", &enteredValue);
        if(enteredValue == 0){
            printf("For the creation of a matrix, enter 1\n");
            printf("For the modification of a matrix element, enter 2\n");
            printf("For the addition of matrices, enter 3\n");
            printf("For the multiplication of matrices, enter 4\n");
            printf("For the scalar multiplication of a matrix, enter 5\n");
            printf("For the reciept of a matrix element, enter 6\n");
            printf("For the reciept of a matrix, enter 7\n");
            printf("For exit, enter 8\n");
        }else if(enteredValue == 1){
            matrixPreviousSize = matrixSize;
            printf("Enter the size of the matrix\n");
            scanf("%d", &matrixSize);
            if(matrixSize < 1){
                printf("Too little number\n");
                matrixSize = matrixPreviousSize;
                continue;
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
                continue;
            }
            if(matrixTypeAlgebra){
                free(matrixTypeAlgebra);
            }
            if(matrixType == 1){
                matrixTypeAlgebra = getIntegerAlgebra();
                scanAndAssign = scanAndAssignInt;
                scanAndModify = scanAndModifyInt;
            }else if(matrixType == 2){
                matrixTypeAlgebra = getRealNumberAlgebra();
                scanAndAssign = scanAndAssignR;
                scanAndModify = scanAndModifyR;
            }else{
                matrixTypeAlgebra = getComplexNumberAlgebra();
                scanAndAssign = scanAndAssignC;
                scanAndModify = scanAndModifyC;
            }
            if(matrix){
                freeMatrix(matrix, diagonalsArray, matrixSize);
            }
            if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                printf("Bad allocation\n");
                continue;
            }
            if(!(allocateMatrix(&matrix, diagonalsArray, matrixSize, matrixTypeAlgebra))){
                printf("Bad allocation\n");
                free(ptrsub);
                continue;
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
        }else if(enteredValue == 2){
            if(!matrix){
                printf("The matrix does not exist\n");
                continue;
            }
            printf("Enter the row and the column of the element (starting the count from 0)\n");
            scanf("%d", &unssub1);
            scanf("%d", &unssub2);
            if(unssub1 >= matrixSize || unssub2 >= matrixSize){
                printf("The position does not exist\n");
                continue;
            }
            if((int)(unssub1 - unssub2) > 1 || (int)(unssub1 - unssub2) < -1){
                printf("The position is out of the three diagonals\n");
                continue;
            }
            if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                printf("Bad allocation\n");
                continue;
            }
            printf("Enter the new value\n");
            if(matrixType == 3){
                printComplexNumbersNotification();
            }
            scanAndModify(ptrsub, unssub1, unssub2, matrix, matrixTypeAlgebra);
            free(ptrsub);
        }else if(enteredValue == 3){
            if(!matrix){
                printf("The matrix does not exist\n");
                continue;
            }
            if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                printf("Bad allocation\n");
                continue;
            }
            if(!(allocateMatrix(&matrixSub, diagonalsArraySub, matrixSize, matrixTypeAlgebra))){
                printf("Bad allocation\n");
                free(ptrsub);
                continue;
            }
            if(!(allocateMatrix(&matrixInterm, diagonalsArrayInterm, matrixSize, matrixTypeAlgebra))){
                printf("Bad allocation\n");
                freeMatrix(matrixSub, diagonalsArraySub, matrixSize);
                free(ptrsub);
                continue;
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
            freeMatrix(matrixSub, diagonalsArraySub, matrixSize);
            free(ptrsub);
            freeMatrix(matrix, diagonalsArray, matrixSize);
            matrix = matrixInterm;
            matrixInterm = NULL;
            ptrsub = (void*)diagonalsArray;
            diagonalsArray = diagonalsArrayInterm;
            diagonalsArrayInterm = (void***)ptrsub;
            ptrsub = NULL;
        }else if(enteredValue == 4){
            if(!matrix){
                printf("The matrix does not exist\n");
                continue;
            }
            if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                printf("Bad allocation\n");
                continue;
            }
            if(!(allocateMatrix(&matrixSub, diagonalsArraySub, matrixSize, matrixTypeAlgebra))){
                printf("Bad allocation\n");
                free(ptrsub);
                continue;
            }
            if(!(allocateMatrix(&matrixInterm, diagonalsArrayInterm, matrixSize, matrixTypeAlgebra))){
                printf("Bad allocation\n");
                freeMatrix(matrixSub, diagonalsArraySub, matrixSize);
                free(ptrsub);
                continue;
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
                freeMatrix(matrixInterm, diagonalsArrayInterm, matrixSize);
                freeMatrix(matrixSub, diagonalsArraySub, matrixSize);
                free(ptrsub);
                continue;
            }
            freeMatrix(matrixSub, diagonalsArraySub, matrixSize);
            free(ptrsub);
            freeMatrix(matrix, diagonalsArray, matrixSize);
            matrix = matrixInterm;
            matrixInterm = NULL;
            ptrsub = (void*)diagonalsArray;
            diagonalsArray = diagonalsArrayInterm;
            diagonalsArrayInterm = (void***)ptrsub;
            ptrsub = NULL;
        }else if(enteredValue == 5){
            if(!matrix){
                printf("The matrix does not exist\n");
                continue;
            }
            if(!(ptrsub = malloc(matrixTypeAlgebra->elementSize))){
                printf("Bad allocation\n");
                continue;
            }
            if(!(allocateMatrix(&matrixInterm, diagonalsArrayInterm, matrixSize, matrixTypeAlgebra))){
                printf("Bad allocation\n");
                free(ptrsub);
                continue;
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
            freeMatrix(matrix, diagonalsArray, matrixSize);
            matrix = matrixInterm;
            matrixInterm = NULL;
            ptrsub = (void*)diagonalsArray;
            diagonalsArray = diagonalsArrayInterm;
            diagonalsArrayInterm = (void***)ptrsub;
            ptrsub = NULL;
        }else if(enteredValue == 6){
            if(!matrix){
                printf("The matrix does not exist\n");
                continue;
            }
            printf("Enter the row and the column of the element (starting the count from 0)\n");
            scanf("%d", &unssub1);
            scanf("%d", &unssub2);
            if(unssub1 >= matrixSize || unssub2 >= matrixSize){
                printf("The position does not exist\n");
                continue;
            }
            printTridiagonalMatrixElement(unssub1, unssub2, matrix, matrixTypeAlgebra);
            printf("\n");
        }else if(enteredValue == 7){
            if(!matrix){
                printf("The matrix does not exist\n");
                continue;
            }
            for(unsigned int row = 0, column; row < matrixSize; row++){
                for(column = 0; column < matrixSize - 1; column++){
                    printTridiagonalMatrixElement(row, column, matrix, matrixTypeAlgebra);
                    printf(" ");
                }
                printTridiagonalMatrixElement(row, column, matrix, matrixTypeAlgebra);
                printf("\n");
            }
        }else{
            break;
        }
    }
    if(matrix) freeMatrix(matrix, diagonalsArray, matrixSize);
    free(diagonalsArray);
    free(diagonalsArraySub);
    free(diagonalsArrayInterm);
    return 0;
}