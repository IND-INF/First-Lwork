#ifndef _REAL_NUMBER_ALGEBRA_H_
#define _REAL_NUMBER_ALGEBRA_H_

#include "scalar_algebra.h"

const Algebra *getRealNumberAlgebra(void);

void assignRealNumber(void *element, double value);
void recieveRealNumber(void *element, double *value);

#endif //_REAL_NUMBER_ALGEBRA_H_