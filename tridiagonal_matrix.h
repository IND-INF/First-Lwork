#ifndef _TRIDIAGONAL_MATRIX_H_
#define _TRIDIAGONAL_MATRIX_H_

#include "scalar_algebra.h"

typedef struct _struct_TriMatrix TriMatrix;

int addTridiagonalMatrices(const TriMatrix *m1, const TriMatrix *m2, TriMatrix *res);
int scalarlyMultiplyTridiagonalMatrix(const void *scalar, const TriMatrix *mat, TriMatrix *res,
            const Algebra *scalarAlgebra);

int allocTridiagonalMatrix(TriMatrix **mat, size_t matrixSize, const Algebra *scalarAlgebra);
void freeTridiagonalMatrix(TriMatrix *mat);
int initializeTridiagonalMatrix(TriMatrix *mat, const void *scalar, const Algebra *scalarAlgebra);

int setTridiagonalMatrixElement(size_t row, size_t column, TriMatrix *mat,
            const void *scalar, const Algebra *scalarAlgebra);
int getTridiagonalMatrixElement(size_t row, size_t column, const TriMatrix *mat,
            void *scalar, const Algebra *scalarAlgebra);

int scanTridiagonalMatrixElement(size_t row, size_t column, TriMatrix *mat);
int sScanTridiagonalMatrixElement(const char *input, size_t row, size_t column, TriMatrix *mat);
int sNScanTridiagonalMatrixElement(const char *input, size_t row, size_t column, TriMatrix *mat, int *stopIndex);
int printTridiagonalMatrixElement(size_t row, size_t column, const TriMatrix *mat);

#endif //_TRIDIAGONAL_MATRIX_H_