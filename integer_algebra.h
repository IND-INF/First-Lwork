#ifndef _INTEGER_ALGEBRA_H_
#define _INTEGER_ALGEBRA_H_

#include "scalar_algebra.h"

const Algebra *getIntegerAlgebra(void);
void assignInteger(void *element, int value);
void recieveInteger(void *element, int *value);

#endif
