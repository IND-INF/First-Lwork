#include <stdio.h>
#include <stdlib.h>
#include "tridiagonal_matrix.h"
#include "real_number_algebra.h"
#include "complex_number_algebra.h"

#define TESTING_MACRO(condition)\
do{                             \
    if(!(condition)) failed = 1;\
}while(0);

#define COUNTER_OF_TESTS_MACRO(testName)\
do{                                     \
    res = testName();                   \
    if(res == -1){                      \
        printf(#testName);              \
        printf(" failed BadAlloc\n");   \
    }else if(!res){                     \
        printf(#testName);              \
        printf(" failed WA\n");         \
    }else passed++;                     \
}while(0);

int TestRealNumberSize(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    TESTING_MACRO(alg->elementSize == sizeof(double))

    return (!failed);

}
int TestRealNumberSet(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;

    setRealNumber(ptrToElement, 10.5);
    TESTING_MACRO(*((double*)ptrToElement) == 10.5)

    setRealNumber(ptrToElement, -8);
    TESTING_MACRO(*((double*)ptrToElement) == -8)

    setRealNumber(ptrToElement, 0);
    TESTING_MACRO(*((double*)ptrToElement) == 0)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberGetFraction(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    double receipt;

    setRealNumber(ptrToElement, 6.5);

    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == 6.5)
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == 6.5)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberGetNegative(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    double receipt;

    setRealNumber(ptrToElement, -19);
    
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == -19)
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == -19)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberGetZero(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    double receipt;

    setRealNumber(ptrToElement, 0);
    
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == 0)
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == 0)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberStringScanFraction(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;

    char str[10] = " 12.5 cat";
    alg->sScan(str, ptrToElement);
    TESTING_MACRO(*((double*)ptrToElement) == 12.5)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberStringScanNegative(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;

    char str[4] = "-17";
    alg->sScan(str, ptrToElement);
    TESTING_MACRO(*((double*)ptrToElement) == -17)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberStringScanZero(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;

    char str[2] = "0";
    alg->sScan(str, ptrToElement);
    TESTING_MACRO(*((double*)ptrToElement) == 0)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberStringNScanFraction(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    int receipt;

    char str[10] = " 12.5 cat";
    alg->sNScan(str, ptrToElement, &receipt);
    TESTING_MACRO(*((double*)ptrToElement) == 12.5)
    TESTING_MACRO(receipt == 5)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberStringNScanNegative(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    int receipt;

    char str[4] = "-17";
    alg->sNScan(str, ptrToElement, &receipt);
    TESTING_MACRO(*((double*)ptrToElement) == -17)
    TESTING_MACRO(receipt == 3)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberStringNScanZero(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    int receipt;

    char str[2] = "0";
    alg->sNScan(str, ptrToElement, &receipt);
    TESTING_MACRO(*((double*)ptrToElement) == 0)
    TESTING_MACRO(receipt == 1)

    free(ptrToElement);
    return (!failed);

}
int TestRealNumberAddFraction(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;
    
    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receipt;

    setRealNumber(ptr1, 3);
    setRealNumber(ptr2, 2.5);

    alg->add(ptr1, ptr2, ptrToElement);
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == 5.5);

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestRealNumberAddNegative(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;
    
    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receipt;

    setRealNumber(ptr1, -10);
    setRealNumber(ptr2, 6);

    alg->add(ptr1, ptr2, ptrToElement);
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == -4);

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestRealNumberAddZero(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;
    
    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receipt;

    setRealNumber(ptr1, 0);
    setRealNumber(ptr2, 3.5);

    alg->add(ptr1, ptr2, ptrToElement);
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == 3.5);

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestRealNumberMultiplyFraction(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receipt;
    
    setRealNumber(ptr1, 5);
    setRealNumber(ptr2, 1.5);

    alg->multiply(ptr1, ptr2, ptrToElement);
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == 7.5);

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestRealNumberMultiplyNegative(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receipt;
    
    setRealNumber(ptr1, -4);
    setRealNumber(ptr2, 2);

    alg->multiply(ptr1, ptr2, ptrToElement);
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == -8);

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestRealNumberMultiplyZero(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receipt;
    
    setRealNumber(ptr1, 0);
    setRealNumber(ptr2, 15);

    alg->multiply(ptr1, ptr2, ptrToElement);
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == 0);

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestRealNumberMultiplyOne(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receipt;
    
    setRealNumber(ptr1, 1);
    setRealNumber(ptr2, 5.5);

    alg->multiply(ptr1, ptr2, ptrToElement);
    getRealNumber(ptrToElement, &receipt);
    TESTING_MACRO(receipt == 5.5);

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}

int TestComplexNumberSize(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    TESTING_MACRO(alg->elementSize == 2*sizeof(double))

    return (!failed);

}
int TestComplexNumberSet(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;

    setComplexNumber(ptrToElement, 6.5, 3.25);
    TESTING_MACRO(*((double*)ptrToElement) == 6.5)
    TESTING_MACRO(*((double*)ptrToElement+1) == 3.25)

    setComplexNumber(ptrToElement, -10, -1.5);
    TESTING_MACRO(*((double*)ptrToElement) == -10)
    TESTING_MACRO(*((double*)ptrToElement+1) == -1.5)

    setComplexNumber(ptrToElement, 0, 0);
    TESTING_MACRO(*((double*)ptrToElement) == 0)
    TESTING_MACRO(*((double*)ptrToElement+1) == 0)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberGetFraction(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    double receiptR, receiptI;

    setComplexNumber(ptrToElement, 9.25, 2.5);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);
    TESTING_MACRO(receiptR == 9.25)
    TESTING_MACRO(receiptI == 2.5)

    getComplexNumber(ptrToElement, &receiptR, &receiptI);
    TESTING_MACRO(receiptR == 9.25)
    TESTING_MACRO(receiptI == 2.5)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberGetNegative(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    double receiptR, receiptI;

    setComplexNumber(ptrToElement, -3.5, -14);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);
    TESTING_MACRO(receiptR == -3.5)
    TESTING_MACRO(receiptI == -14)

    getComplexNumber(ptrToElement, &receiptR, &receiptI);
    TESTING_MACRO(receiptR == -3.5)
    TESTING_MACRO(receiptI == -14)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberGetZero(){

    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    double receiptR, receiptI;

    setComplexNumber(ptrToElement, 0, 0);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);
    TESTING_MACRO(receiptR == 0)
    TESTING_MACRO(receiptI == 0)

    getComplexNumber(ptrToElement, &receiptR, &receiptI);
    TESTING_MACRO(receiptR == 0)
    TESTING_MACRO(receiptI == 0)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberStringScanFraction(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;

    char str[15] = " 6.5+9.25i fox";
    alg->sScan(str, ptrToElement);
    TESTING_MACRO(*((double*)ptrToElement) == 6.5)
    TESTING_MACRO(*((double*)ptrToElement+1) == 9.25)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberStringScanNegative(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;

    char str[10] = "-1+-10.5i";
    alg->sScan(str, ptrToElement);
    TESTING_MACRO(*((double*)ptrToElement) == -1)
    TESTING_MACRO(*((double*)ptrToElement+1) == -10.5)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberStringScanZero(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;

    char str[5] = "0+0i";
    alg->sScan(str, ptrToElement);
    TESTING_MACRO(*((double*)ptrToElement) == 0)
    TESTING_MACRO(*((double*)ptrToElement+1) == 0)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberStringNScanFraction(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    int receipt;

    char str[15] = " 6.5+9.25i fox";
    alg->sNScan(str, ptrToElement, &receipt);
    TESTING_MACRO(*((double*)ptrToElement) == 6.5)
    TESTING_MACRO(*((double*)ptrToElement+1) == 9.25)
    TESTING_MACRO(receipt == 10)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberStringNScanNegative(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    int receipt;

    char str[10] = "-1+-10.5i";
    alg->sNScan(str, ptrToElement, &receipt);
    TESTING_MACRO(*((double*)ptrToElement) == -1)
    TESTING_MACRO(*((double*)ptrToElement+1) == -10.5)
    TESTING_MACRO(receipt == 9)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberStringNScanZero(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))) return -1;
    int receipt;

    char str[5] = "0+0i";
    alg->sNScan(str, ptrToElement, &receipt);
    TESTING_MACRO(*((double*)ptrToElement) == 0)
    TESTING_MACRO(*((double*)ptrToElement+1) == 0)
    TESTING_MACRO(receipt == 4)

    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberAddFraction(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receiptR, receiptI;

    setComplexNumber(ptr1, 8.5, 9);
    setComplexNumber(ptr2, 11, 4.75);

    alg->add(ptr1, ptr2, ptrToElement);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);

    TESTING_MACRO(receiptR == 19.5)
    TESTING_MACRO(receiptI == 13.75)

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberAddNegative(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receiptR, receiptI;

    setComplexNumber(ptr1, -1.5, 5);
    setComplexNumber(ptr2, 4, -13);

    alg->add(ptr1, ptr2, ptrToElement);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);

    TESTING_MACRO(receiptR == 2.5)
    TESTING_MACRO(receiptI == -8)

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberAddZero(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receiptR, receiptI;

    setComplexNumber(ptr1, -8, 7.5);
    setComplexNumber(ptr2, 0, 0);

    alg->add(ptr1, ptr2, ptrToElement);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);

    TESTING_MACRO(receiptR == -8)
    TESTING_MACRO(receiptI == 7.5)

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberMultiplyFraction(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receiptR, receiptI;

    setComplexNumber(ptr1, 8.5, 2);
    setComplexNumber(ptr2, 4, 1.5);

    alg->multiply(ptr1, ptr2, ptrToElement);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);

    TESTING_MACRO(receiptR == 31)
    TESTING_MACRO(receiptI == 20.75)

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberMultiplyNegative(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receiptR, receiptI;

    setComplexNumber(ptr1, -4, 3);
    setComplexNumber(ptr2, -10, -3.5);

    alg->multiply(ptr1, ptr2, ptrToElement);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);

    TESTING_MACRO(receiptR == 50.5)
    TESTING_MACRO(receiptI == -16)

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberMultiplyZero(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receiptR, receiptI;

    setComplexNumber(ptr1, -6.5, 7);
    setComplexNumber(ptr2, 0, 0);

    alg->multiply(ptr1, ptr2, ptrToElement);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);

    TESTING_MACRO(receiptR == 0)
    TESTING_MACRO(receiptI == 0)

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}
int TestComplexNumberMultiplyOne(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getComplexNumberAlgebra())) return -1;

    void *ptr1;
    if(!(ptr1 = malloc(alg->elementSize))) return -1;

    void *ptr2;
    if(!(ptr2 = malloc(alg->elementSize))){
        free(ptr1);
        return -1;
    }

    void *ptrToElement;
    if(!(ptrToElement = malloc(alg->elementSize))){
        free(ptr2);
        free(ptr1);
        return -1;
    }

    double receiptR, receiptI;

    setComplexNumber(ptr1, 11.5, -3);
    setComplexNumber(ptr2, 1, 0);

    alg->multiply(ptr1, ptr2, ptrToElement);

    getComplexNumber(ptrToElement, &receiptR, &receiptI);

    TESTING_MACRO(receiptR == 11.5)
    TESTING_MACRO(receiptI == -3)

    free(ptr1);
    free(ptr2);
    free(ptrToElement);
    return (!failed);

}

int TestMatrixAddFraction(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    size_t size = 3;

    void *ptrSub;
    if(!(ptrSub = malloc(alg->elementSize))) return -1;

    TriMatrix *matrix1;
    if(!(allocTridiagonalMatrix(&matrix1, size, alg))){
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrix2;
    if(!(allocTridiagonalMatrix(&matrix2, size, alg))){
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrixRes;
    if(!(allocTridiagonalMatrix(&matrixRes, size, alg))){
        freeTridiagonalMatrix(matrix2);
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    double receipt;

    double arr1[] = {
        1.5, 2.5, 0,
        3, 4.25, 5,
        0, 6.25, 7.25
    };
    double arr2[] = {
        1, 2, 0,
        3.5, 4.5, 5.5,
        0, 6.25, 7.75
    };
    double arrRes[] = {
        2.5, 4.5, 0,
        6.5, 8.75, 10.5,
        0, 12.5, 15
    };

    for(size_t row = 0; row < size-1; row++){

        setRealNumber(ptrSub, arr1[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix1, ptrSub, alg);
        setRealNumber(ptrSub, arr2[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix2, ptrSub, alg);

        setRealNumber(ptrSub, arr1[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix1, ptrSub, alg);
        setRealNumber(ptrSub, arr2[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix2, ptrSub, alg);

        setRealNumber(ptrSub, arr1[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix1, ptrSub, alg);
        setRealNumber(ptrSub, arr2[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix2, ptrSub, alg);

    }
    setRealNumber(ptrSub, arr1[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix1, ptrSub, alg);
    setRealNumber(ptrSub, arr2[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix2, ptrSub, alg);

    addTridiagonalMatrices(matrix1, matrix2, matrixRes);


    for(size_t row = 0; row < size-1; row++){

        getTridiagonalMatrixElement(row, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row] == receipt)
        
        getTridiagonalMatrixElement(row, row+1, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row+1] == receipt)
        
        getTridiagonalMatrixElement(row+1, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[(row+1)*size + row] == receipt)

    }
    getTridiagonalMatrixElement(size-1, size-1, matrixRes, ptrSub, alg);
    getRealNumber(ptrSub, &receipt);
    TESTING_MACRO(arrRes[(size-1)*size+size-1] == receipt)

    free(ptrSub);
    freeTridiagonalMatrix(matrix1);
    freeTridiagonalMatrix(matrix2);
    freeTridiagonalMatrix(matrixRes);
    return (!failed);

}
int TestMatrixAddNegative(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    size_t size = 3;

    void *ptrSub;
    if(!(ptrSub = malloc(alg->elementSize))) return -1;

    TriMatrix *matrix1;
    if(!(allocTridiagonalMatrix(&matrix1, size, alg))){
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrix2;
    if(!(allocTridiagonalMatrix(&matrix2, size, alg))){
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrixRes;
    if(!(allocTridiagonalMatrix(&matrixRes, size, alg))){
        freeTridiagonalMatrix(matrix2);
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    double receipt;

    double arr1[] = {
        -10, -11, 0,
        -1, 2, -3,
        0, -12, -13
    };
    double arr2[] = {
        1, 2.5, 0,
        -3.5, -4, -5.5,
        0, 6.5, 7
    };
    double arrRes[] = {
        -9, -8.5, 0,
        -4.5, -2, -8.5,
        0, -5.5, -6
    };

    for(size_t row = 0; row < size-1; row++){

        setRealNumber(ptrSub, arr1[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix1, ptrSub, alg);
        setRealNumber(ptrSub, arr2[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix2, ptrSub, alg);

        setRealNumber(ptrSub, arr1[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix1, ptrSub, alg);
        setRealNumber(ptrSub, arr2[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix2, ptrSub, alg);

        setRealNumber(ptrSub, arr1[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix1, ptrSub, alg);
        setRealNumber(ptrSub, arr2[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix2, ptrSub, alg);

    }
    setRealNumber(ptrSub, arr1[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix1, ptrSub, alg);
    setRealNumber(ptrSub, arr2[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix2, ptrSub, alg);

    addTridiagonalMatrices(matrix1, matrix2, matrixRes);


    for(size_t row = 0; row < size-1; row++){

        getTridiagonalMatrixElement(row, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row] == receipt)
        
        getTridiagonalMatrixElement(row, row+1, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row+1] == receipt)
        
        getTridiagonalMatrixElement(row+1, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[(row+1)*size + row] == receipt)

    }
    getTridiagonalMatrixElement(size-1, size-1, matrixRes, ptrSub, alg);
    getRealNumber(ptrSub, &receipt);
    TESTING_MACRO(arrRes[(size-1)*size+size-1] == receipt)

    free(ptrSub);
    freeTridiagonalMatrix(matrix1);
    freeTridiagonalMatrix(matrix2);
    freeTridiagonalMatrix(matrixRes);
    return (!failed);

}
int TestMatrixAddZero(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    size_t size = 3;

    void *ptrSub;
    if(!(ptrSub = malloc(alg->elementSize))) return -1;

    TriMatrix *matrix1;
    if(!(allocTridiagonalMatrix(&matrix1, size, alg))){
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrix2;
    if(!(allocTridiagonalMatrix(&matrix2, size, alg))){
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrixRes;
    if(!(allocTridiagonalMatrix(&matrixRes, size, alg))){
        freeTridiagonalMatrix(matrix2);
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    double receipt;

    double arr1[] = {
        10, 11.5, 0,
        -12, -13.5, -14,
        0, 15.5, 16
    };
    double arr2[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    double arrRes[] = {
        10, 11.5, 0,
        -12, -13.5, -14,
        0, 15.5, 16
    };

    for(size_t row = 0; row < size-1; row++){

        setRealNumber(ptrSub, arr1[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix1, ptrSub, alg);
        setRealNumber(ptrSub, arr2[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix2, ptrSub, alg);

        setRealNumber(ptrSub, arr1[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix1, ptrSub, alg);
        setRealNumber(ptrSub, arr2[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix2, ptrSub, alg);

        setRealNumber(ptrSub, arr1[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix1, ptrSub, alg);
        setRealNumber(ptrSub, arr2[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix2, ptrSub, alg);

    }
    setRealNumber(ptrSub, arr1[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix1, ptrSub, alg);
    setRealNumber(ptrSub, arr2[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix2, ptrSub, alg);

    addTridiagonalMatrices(matrix1, matrix2, matrixRes);


    for(size_t row = 0; row < size-1; row++){

        getTridiagonalMatrixElement(row, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row] == receipt)
        
        getTridiagonalMatrixElement(row, row+1, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row+1] == receipt)
        
        getTridiagonalMatrixElement(row+1, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[(row+1)*size + row] == receipt)

    }
    getTridiagonalMatrixElement(size-1, size-1, matrixRes, ptrSub, alg);
    getRealNumber(ptrSub, &receipt);
    TESTING_MACRO(arrRes[(size-1)*size+size-1] == receipt)

    free(ptrSub);
    freeTridiagonalMatrix(matrix1);
    freeTridiagonalMatrix(matrix2);
    freeTridiagonalMatrix(matrixRes);
    return (!failed);

}
int TestMatrixScalarlyMultiplyFraction(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    size_t size = 3;

    void *ptrSub;
    if(!(ptrSub = malloc(alg->elementSize))) return -1;

    TriMatrix *matrix1;
    if(!(allocTridiagonalMatrix(&matrix1, size, alg))){
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrixRes;
    if(!(allocTridiagonalMatrix(&matrixRes, size, alg))){
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    double receipt;

    double arr1[] = {
        1.5, 2, 0,
        3, 4.5, 5,
        0, 6, 7.5
    };
    double arrRes[] = {
        0.75, 1, 0,
        1.5, 2.25, 2.5,
        0, 3, 3.75
    };

    for(size_t row = 0; row < size-1; row++){

        setRealNumber(ptrSub, arr1[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix1, ptrSub, alg);

        setRealNumber(ptrSub, arr1[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix1, ptrSub, alg);

        setRealNumber(ptrSub, arr1[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix1, ptrSub, alg);

    }
    setRealNumber(ptrSub, arr1[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix1, ptrSub, alg);

    setRealNumber(ptrSub, 0.5);

    scalarlyMultiplyTridiagonalMatrix(ptrSub, matrix1, matrixRes, alg);


    for(size_t row = 0; row < size-1; row++){

        getTridiagonalMatrixElement(row, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row] == receipt)
        
        getTridiagonalMatrixElement(row, row+1, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row+1] == receipt)
        
        getTridiagonalMatrixElement(row+1, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[(row+1)*size + row] == receipt)

    }
    getTridiagonalMatrixElement(size-1, size-1, matrixRes, ptrSub, alg);
    getRealNumber(ptrSub, &receipt);
    TESTING_MACRO(arrRes[(size-1)*size+size-1] == receipt)

    free(ptrSub);
    freeTridiagonalMatrix(matrix1);
    freeTridiagonalMatrix(matrixRes);
    return (!failed);

}
int TestMatrixScalarlyMultiplyNegative(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    size_t size = 3;

    void *ptrSub;
    if(!(ptrSub = malloc(alg->elementSize))) return -1;

    TriMatrix *matrix1;
    if(!(allocTridiagonalMatrix(&matrix1, size, alg))){
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrixRes;
    if(!(allocTridiagonalMatrix(&matrixRes, size, alg))){
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    double receipt;

    double arr1[] = {
        16, 15.5, 0,
        -14.5, -13, -12.5,
        0, 11.5, 10
    };
    double arrRes[] = {
        -32, -31, 0,
        29, 26, 25,
        0, -23, -20
    };

    for(size_t row = 0; row < size-1; row++){

        setRealNumber(ptrSub, arr1[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix1, ptrSub, alg);

        setRealNumber(ptrSub, arr1[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix1, ptrSub, alg);

        setRealNumber(ptrSub, arr1[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix1, ptrSub, alg);

    }
    setRealNumber(ptrSub, arr1[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix1, ptrSub, alg);

    setRealNumber(ptrSub, -2);

    scalarlyMultiplyTridiagonalMatrix(ptrSub, matrix1, matrixRes, alg);


    for(size_t row = 0; row < size-1; row++){

        getTridiagonalMatrixElement(row, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row] == receipt)
        
        getTridiagonalMatrixElement(row, row+1, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row+1] == receipt)
        
        getTridiagonalMatrixElement(row+1, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[(row+1)*size + row] == receipt)

    }
    getTridiagonalMatrixElement(size-1, size-1, matrixRes, ptrSub, alg);
    getRealNumber(ptrSub, &receipt);
    TESTING_MACRO(arrRes[(size-1)*size+size-1] == receipt)

    free(ptrSub);
    freeTridiagonalMatrix(matrix1);
    freeTridiagonalMatrix(matrixRes);
    return (!failed);

}
int TestMatrixScalarlyMultiplyZero(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    size_t size = 3;

    void *ptrSub;
    if(!(ptrSub = malloc(alg->elementSize))) return -1;

    TriMatrix *matrix1;
    if(!(allocTridiagonalMatrix(&matrix1, size, alg))){
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrixRes;
    if(!(allocTridiagonalMatrix(&matrixRes, size, alg))){
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    double receipt;

    double arr1[] = {
        -9, 8.5, 0,
        7, -6.5, 5,
        0, 4.5, -3
    };
    double arrRes[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };

    for(size_t row = 0; row < size-1; row++){

        setRealNumber(ptrSub, arr1[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix1, ptrSub, alg);

        setRealNumber(ptrSub, arr1[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix1, ptrSub, alg);

        setRealNumber(ptrSub, arr1[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix1, ptrSub, alg);

    }
    setRealNumber(ptrSub, arr1[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix1, ptrSub, alg);

    setRealNumber(ptrSub, 0);

    scalarlyMultiplyTridiagonalMatrix(ptrSub, matrix1, matrixRes, alg);


    for(size_t row = 0; row < size-1; row++){

        getTridiagonalMatrixElement(row, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row] == receipt)
        
        getTridiagonalMatrixElement(row, row+1, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row+1] == receipt)
        
        getTridiagonalMatrixElement(row+1, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[(row+1)*size + row] == receipt)

    }
    getTridiagonalMatrixElement(size-1, size-1, matrixRes, ptrSub, alg);
    getRealNumber(ptrSub, &receipt);
    TESTING_MACRO(arrRes[(size-1)*size+size-1] == receipt)

    free(ptrSub);
    freeTridiagonalMatrix(matrix1);
    freeTridiagonalMatrix(matrixRes);
    return (!failed);

}
int TestMatrixScalarlyMultiplyOne(){
    
    int failed = 0;
    const Algebra *alg;
    if(!(alg = getRealNumberAlgebra())) return -1;

    size_t size = 3;

    void *ptrSub;
    if(!(ptrSub = malloc(alg->elementSize))) return -1;

    TriMatrix *matrix1;
    if(!(allocTridiagonalMatrix(&matrix1, size, alg))){
        free(ptrSub);
        return -1;
    }

    TriMatrix *matrixRes;
    if(!(allocTridiagonalMatrix(&matrixRes, size, alg))){
        freeTridiagonalMatrix(matrix1);
        free(ptrSub);
        return -1;
    }

    double receipt;

    double arr1[] = {
        11.5, -12, 0,
        13, -14.5, 15,
        0, -16, 17.5
    };
    double arrRes[] = {
        11.5, -12, 0,
        13, -14.5, 15,
        0, -16, 17.5
    };

    for(size_t row = 0; row < size-1; row++){

        setRealNumber(ptrSub, arr1[row*size + row]);
        setTridiagonalMatrixElement(row, row, matrix1, ptrSub, alg);

        setRealNumber(ptrSub, arr1[row*size + row+1]);
        setTridiagonalMatrixElement(row, row+1, matrix1, ptrSub, alg);

        setRealNumber(ptrSub, arr1[(row+1)*size + row]);
        setTridiagonalMatrixElement(row+1, row, matrix1, ptrSub, alg);

    }
    setRealNumber(ptrSub, arr1[(size-1)*size + (size-1)]);
    setTridiagonalMatrixElement(size-1, size-1, matrix1, ptrSub, alg);

    setRealNumber(ptrSub, 1);

    scalarlyMultiplyTridiagonalMatrix(ptrSub, matrix1, matrixRes, alg);


    for(size_t row = 0; row < size-1; row++){

        getTridiagonalMatrixElement(row, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row] == receipt)
        
        getTridiagonalMatrixElement(row, row+1, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[row*size + row+1] == receipt)
        
        getTridiagonalMatrixElement(row+1, row, matrixRes, ptrSub, alg);
        getRealNumber(ptrSub, &receipt);
        TESTING_MACRO(arrRes[(row+1)*size + row] == receipt)

    }
    getTridiagonalMatrixElement(size-1, size-1, matrixRes, ptrSub, alg);
    getRealNumber(ptrSub, &receipt);
    TESTING_MACRO(arrRes[(size-1)*size+size-1] == receipt)

    free(ptrSub);
    freeTridiagonalMatrix(matrix1);
    freeTridiagonalMatrix(matrixRes);
    return (!failed);

}

void doTheTest(void){

    int passed = 0;
    int res;

    COUNTER_OF_TESTS_MACRO(TestRealNumberSize)
    COUNTER_OF_TESTS_MACRO(TestRealNumberSet)
    COUNTER_OF_TESTS_MACRO(TestRealNumberGetFraction)
    COUNTER_OF_TESTS_MACRO(TestRealNumberGetNegative)
    COUNTER_OF_TESTS_MACRO(TestRealNumberGetZero)
    COUNTER_OF_TESTS_MACRO(TestRealNumberStringScanFraction)
    COUNTER_OF_TESTS_MACRO(TestRealNumberStringScanNegative)
    COUNTER_OF_TESTS_MACRO(TestRealNumberStringScanZero)
    COUNTER_OF_TESTS_MACRO(TestRealNumberStringNScanFraction)
    COUNTER_OF_TESTS_MACRO(TestRealNumberStringNScanNegative)
    COUNTER_OF_TESTS_MACRO(TestRealNumberStringNScanZero)
    COUNTER_OF_TESTS_MACRO(TestRealNumberAddFraction)
    COUNTER_OF_TESTS_MACRO(TestRealNumberAddNegative)
    COUNTER_OF_TESTS_MACRO(TestRealNumberAddZero)
    COUNTER_OF_TESTS_MACRO(TestRealNumberMultiplyFraction)
    COUNTER_OF_TESTS_MACRO(TestRealNumberMultiplyNegative)
    COUNTER_OF_TESTS_MACRO(TestRealNumberMultiplyZero)
    COUNTER_OF_TESTS_MACRO(TestRealNumberMultiplyOne)

    COUNTER_OF_TESTS_MACRO(TestComplexNumberSize)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberSet)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberGetFraction)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberGetNegative)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberGetZero)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberStringScanFraction)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberStringScanNegative)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberStringScanZero)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberStringNScanFraction)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberStringNScanNegative)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberStringNScanZero)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberAddFraction)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberAddNegative)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberAddZero)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberMultiplyFraction)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberMultiplyNegative)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberMultiplyZero)
    COUNTER_OF_TESTS_MACRO(TestComplexNumberMultiplyOne)

    COUNTER_OF_TESTS_MACRO(TestMatrixAddFraction)
    COUNTER_OF_TESTS_MACRO(TestMatrixAddNegative)
    COUNTER_OF_TESTS_MACRO(TestMatrixAddZero)
    COUNTER_OF_TESTS_MACRO(TestMatrixScalarlyMultiplyFraction)
    COUNTER_OF_TESTS_MACRO(TestMatrixScalarlyMultiplyNegative)
    COUNTER_OF_TESTS_MACRO(TestMatrixScalarlyMultiplyZero)
    COUNTER_OF_TESTS_MACRO(TestMatrixScalarlyMultiplyOne)

    printf("%d of 43 tests passed successfully\n", passed);

}