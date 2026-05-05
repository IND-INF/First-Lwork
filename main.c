#include <stdio.h>
#include <stdlib.h>
#include "tridiagonal_matrix.h"
#include "real_number_algebra.h"
#include "complex_number_algebra.h"
#include "test_of_the_structure.h"

#define INPUT_SIZE_MACRO (100)

#define SINGLE_INPUT_MACRO(sScanExpression)                                             \
do{                                                                                     \
    if(fgets(input, INPUT_SIZE_MACRO, stdin)){                                          \
        inputRes = (sScanExpression);                                                   \
        if(!inputRes){                                                                  \
            printf("Wrong input, enter a number with the western arabic numerals.\n");  \
        }else if(inputRes > 0) break;                                                   \
    }                                                                                   \
}while(1);

#define LOOP_INPUT_MACRO(initialization, testCondition, updateExpression, sNScanExpression) \
do{                                                                                         \
    (initialization);                                                                       \
    do{                                                                                     \
        if(fgets(input, INPUT_SIZE_MACRO, stdin)){                                          \
            inputFromIndex = input;                                                         \
            while((testCondition) && (inputRes = (sNScanExpression)) > 0){                  \
                (updateExpression);                                                         \
                inputFromIndex += inputStopIndex;                                           \
            }                                                                               \
            if(!inputRes){                                                                  \
                printf("Wrong input, enter a number with the western arabic numerals.\n");  \
            }else if(!(testCondition)) break;                                               \
        }                                                                                   \
    }while(1);                                                                              \
}while(0);

int main(void){

    doTheTest();

    int exitFlag = 0;

    char input[INPUT_SIZE_MACRO];
    int inputRes;
    int inputStopIndex;
    char *inputFromIndex;

    int enteredValue = 0;

    size_t matrixSize = 0;
    int matrixType = 0;
    const Algebra *matrixTypeAlgebra = NULL;

    size_t sizeSub1;
    size_t sizeSub2;
    int intSub;
    const Algebra *algSub = NULL;
    void *ptrSub;

    TriMatrix *matrix = NULL;
    TriMatrix *matrixSub = NULL;
    TriMatrix *matrixInterm = NULL;

    do{

        printf("For assistance, enter 0.\n");

        SINGLE_INPUT_MACRO(sscanf(input, "%d", &enteredValue))

        switch(enteredValue){
            case 0:
                printf("For the creation of a matrix, enter 1.\n");
                printf("For the modification of a matrix element, enter 2.\n");
                printf("For the addition of matrices, enter 3.\n");
                printf("For the scalar multiplication of a matrix, enter 4.\n");
                printf("For the receipt of a matrix element, enter 5.\n");
                printf("For the receipt of a matrix, enter 6.\n");
                printf("For exit, enter 7.\n");
                break;
            case 1:

                printf("Enter the size of the matrix.\n");

                SINGLE_INPUT_MACRO(sscanf(input, "%lld", &sizeSub1))

                if(sizeSub1 < 1){
                    printf("Too little number.\n");
                    break;
                }

                printf("Enter the type of the matrix:\n");
                printf("- For the real number type, enter 1;\n");
                printf("- For the complex number type, enter 2.\n");

                SINGLE_INPUT_MACRO(sscanf(input, "%d", &intSub))

                if(intSub != 1 && intSub != 2){
                    printf("Wrong number.\n");
                    break;
                }

                if(intSub == 1){
                    if(!(algSub = getRealNumberAlgebra())){
                        printf("Bad algebra allocation.\n");
                        break;
                    }
                }else{
                    if(!(algSub = getComplexNumberAlgebra())){
                        printf("Bad algebra allocation.\n");
                        break;
                    }
                }
                
                matrixSize = sizeSub1;
                matrixType = intSub;
                matrixTypeAlgebra = algSub;

                if(matrix){
                    freeTridiagonalMatrix(matrix);
                }
                if(!(allocTridiagonalMatrix(&matrix, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation.\n");
                    matrix = NULL;
                    break;
                }

                if(matrixType == 2){
                    printf("Enter numbers in the form 'a+bi'.\n");
                }
                
                if(matrixSize > 1){

                    printf("Enter the superior diagonal of the matrix.\n");

                    LOOP_INPUT_MACRO(sizeSub1 = 0, sizeSub1 < matrixSize - 1, sizeSub1++,
                                sNScanTridiagonalMatrixElement(inputFromIndex, sizeSub1, sizeSub1+1, matrix,
                                &inputStopIndex))
                    
                }

                printf("Enter the principal diagonal of the matrix.\n");
                
                LOOP_INPUT_MACRO(sizeSub1 = 0, sizeSub1 < matrixSize, sizeSub1++,
                            sNScanTridiagonalMatrixElement(inputFromIndex, sizeSub1, sizeSub1, matrix,
                            &inputStopIndex))

                if(matrixSize > 1){

                    printf("Enter the inferior diagonal of the matrix.\n");

                    LOOP_INPUT_MACRO(sizeSub1 = 0, sizeSub1 < matrixSize - 1, sizeSub1++,
                                sNScanTridiagonalMatrixElement(inputFromIndex, sizeSub1+1, sizeSub1, matrix,
                                &inputStopIndex))

                }

                break;

            case 2:

                if(!matrix){
                    printf("The matrix does not exist.\n");
                    break;
                }

                printf("Enter the row and the column of the element (starting the count from 0).\n");

                SINGLE_INPUT_MACRO(sscanf(input, "%lld", &sizeSub1))

                SINGLE_INPUT_MACRO(sscanf(input, "%lld", &sizeSub2))

                if(sizeSub1 >= matrixSize || sizeSub2 >= matrixSize){
                    printf("The position does not exist.\n");
                    break;
                }
                if((int)(sizeSub1 - sizeSub2) > 1 || (int)(sizeSub1 - sizeSub2) < -1){
                    printf("The position is out of the three diagonals.\n");
                    break;
                }

                printf("Notification: the matrix type is ");
                if(matrixType == 1){
                    printf("the real number type.\n");
                }else{
                    printf("the complex number type.\n");
                    printf("Enter numbers in the form 'a+bi'.\n");
                }

                printf("Enter the new value.\n");
                
                SINGLE_INPUT_MACRO(sScanTridiagonalMatrixElement(input, sizeSub1, sizeSub2, matrix))

                break;

            case 3:

                if(!matrix){
                    printf("The matrix does not exist.\n");
                    break;
                }

                if(!(allocTridiagonalMatrix(&matrixSub, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation.\n");
                    matrixSub = NULL;
                    break;
                }
                if(!(allocTridiagonalMatrix(&matrixInterm, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation.\n");
                    matrixInterm = NULL;
                    freeTridiagonalMatrix(matrixSub);
                    matrixSub = NULL;
                    break;
                }

                printf("Notification: the matrix size is %lld, the matrix type is ", matrixSize);
                if(matrixType == 1){
                    printf("the real number type.\n");
                }else{
                    printf("the complex number type.\n");
                    printf("Enter numbers in the form 'a+bi'.\n");
                }

                if(matrixSize > 1){

                    printf("Enter the superior diagonal of the matrix you want the current matrix to be added to.\n");
                    
                    LOOP_INPUT_MACRO(sizeSub1 = 0, sizeSub1 < matrixSize - 1, sizeSub1++,
                                sNScanTridiagonalMatrixElement(inputFromIndex, sizeSub1, sizeSub1+1, matrixSub,
                                &inputStopIndex))
                    
                }

                printf("Enter the principal diagonal of the matrix you want the current matrix to be added to.\n");
                
                LOOP_INPUT_MACRO(sizeSub1 = 0, sizeSub1 < matrixSize, sizeSub1++,
                            sNScanTridiagonalMatrixElement(inputFromIndex, sizeSub1, sizeSub1, matrixSub,
                            &inputStopIndex))

                if(matrixSize > 1){

                    printf("Enter the inferior diagonal of the matrix you want the current matrix to be added to.\n");

                    LOOP_INPUT_MACRO(sizeSub1 = 0, sizeSub1 < matrixSize - 1, sizeSub1++,
                                sNScanTridiagonalMatrixElement(inputFromIndex, sizeSub1+1, sizeSub1, matrixSub,
                                &inputStopIndex))

                }

                addTridiagonalMatrices(matrix, matrixSub, matrixInterm);

                freeTridiagonalMatrix(matrixSub);
                matrixSub = NULL;
                freeTridiagonalMatrix(matrix);

                matrix = matrixInterm;
                matrixInterm = NULL;

                break;

            case 4:

                if(!matrix){
                    printf("The matrix does not exist.\n");
                    break;
                }

                if(!(ptrSub = malloc(matrixTypeAlgebra->elementSize))){
                    printf("Bad allocation.\n");
                    break;
                }
                if(!(allocTridiagonalMatrix(&matrixInterm, matrixSize, matrixTypeAlgebra))){
                    printf("Bad allocation.\n");
                    matrixInterm = NULL;
                    free(ptrSub);
                    break;
                }

                printf("Notification: the matrix type is ");
                if(matrixType == 1){
                    printf("the real number type.\n");
                }else{
                    printf("the complex number type.\n");
                    printf("Enter numbers in the form 'a+bi'.\n");
                }

                printf("Enter the scalar you want to multiply by the current matrix.\n");
                
                SINGLE_INPUT_MACRO(matrixTypeAlgebra->sScan(input, ptrSub))

                scalarlyMultiplyTridiagonalMatrix(ptrSub, matrix, matrixInterm, matrixTypeAlgebra);

                free(ptrSub);
                freeTridiagonalMatrix(matrix);

                matrix = matrixInterm;
                matrixInterm = NULL;

                break;

            case 5:

                if(!matrix){
                    printf("The matrix does not exist.\n");
                    break;
                }

                printf("Enter the row and the column of the element (starting the count from 0).\n");
                
                SINGLE_INPUT_MACRO(sscanf(input, "%lld", &sizeSub1))

                SINGLE_INPUT_MACRO(sscanf(input, "%lld", &sizeSub2))

                if(sizeSub1 >= matrixSize || sizeSub2 >= matrixSize){
                    printf("The position does not exist.\n");
                    break;
                }

                printTridiagonalMatrixElement(sizeSub1, sizeSub2, matrix);
                printf("\n");

                break;

            case 6:
            
                if(!matrix){
                    printf("The matrix does not exist.\n");
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
                break;
        }
    }while(!exitFlag);

    if(matrix) freeTridiagonalMatrix(matrix);
    if(matrixSub) freeTridiagonalMatrix(matrixSub);
    if(matrixInterm) freeTridiagonalMatrix(matrixInterm);
    return 0;

}