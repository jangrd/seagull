#ifndef SGL_VECTOR_H
#define SGL_VECTOR_H

// forward declare to avoid circular deps instead of:
//  #include "../SGL_Element/SGL_Element.h"
typedef struct SGL_Element SGL_Element;

#include <stdio.h>
#include <stdlib.h>

// TODO: have sglvector possibly use an abstraction for the pointer
// instead of returning a pointer from vectoradd

typedef struct SGL_Vector {
    SGL_Element** elements;
    size_t count;
    size_t capacity;
    size_t increment;
} SGL_Vector;

SGL_Vector* SGL_VectorNew(size_t initial_size, size_t increment);
SGL_Vector* SGL_VectorAdd(SGL_Vector* vector, SGL_Element* element);

#endif
