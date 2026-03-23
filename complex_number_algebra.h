#ifndef _COMPLEX_NUMBER_ALGEBRA_H_
#define _COMPLEX_NUMBER_ALGEBRA_H_

#include "scalar_algebra.h"

const Algebra *getComplexNumberAlgebra(void);

void assignComplexNumber(void *element, double realPartValue, double imaginaryPartValue);
void recieveComplexNumber(void *element, double *realPartValue, double *imaginaryPartValue);

#endif //_COMPLEX_NUMBER_ALGEBRA_H_