#ifndef SGL_LIB_H
#define SGL_LIB_H

#include <string.h>
// TODO: remove this after proper error handling
#include <stdio.h>

// const char* strstrip(const char* src);
const char* fetchNextByDelim(const char* src, char* result, char delimiter);

#endif
