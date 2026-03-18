#ifndef _SCALAR_ALGEBRA_H_
#define _SCALAR_ALGEBRA_H_

#include <stddef.h>

typedef void (*BinaryOperation)(const void*, const void*, void*);
typedef void (*PrintOperation)(const void*);

typedef struct _struct_Algebra{
    BinaryOperation add;
    BinaryOperation multiply;
    PrintOperation print;
    size_t elementSize;
} Algebra;

#endif //_SCALAR_ALGEBRA_H_
