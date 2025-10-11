#include "SGL_Vector.h"

SGL_Vector* SGL_VectorNew(size_t initial_size, size_t increment) {
    SGL_Vector* target = (SGL_Vector*)malloc(sizeof(SGL_Vector));
    if (target == NULL) {
        fprintf(stderr, "SGL_VectorNew: malloc for SGL_Vector failed\n");
        return NULL;
    }
    target->elements = (SGL_Element**)malloc(initial_size * sizeof(SGL_Element*));
    if (target->elements == NULL) {
        fprintf(stderr, "SGL_VectorNew: malloc for SGL_Element** failed\n");
        free(target);
        return NULL;
    }
    target->count = 0;
    target->capacity = initial_size;
    target->increment = increment;
    return target;
}

SGL_Vector* SGL_VectorAdd(SGL_Vector* vector, SGL_Element* element) {
    if (vector->count == vector->capacity) {
        // TODO: create new vector of size capacity+increment and copy all elements
    }
    vector->elements[vector->count] = element;
    vector->count += 1;
    return vector;
}
