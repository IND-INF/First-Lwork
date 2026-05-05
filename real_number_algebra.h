#ifndef _REAL_NUMBER_ALGEBRA_H_
#define _REAL_NUMBER_ALGEBRA_H_

#include "scalar_algebra.h"

const Algebra *getRealNumberAlgebra(void);

void setRealNumber(void *element, double value);
void getRealNumber(const void *element, double *value);

#endif //_REAL_NUMBER_ALGEBRA_H_