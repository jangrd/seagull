#ifndef SGL_ELEMENT_H
#define SGL_ELEMENT_H

#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "../SGL_Vector/SGL_Vector.h"
#include "../SGL_Lib/SGL_Lib.h"

typedef struct SGL_ElementRect {
    SDL_FRect outer;
    SDL_FRect border;
    SDL_FRect inner;
} SGL_ElementRect;

typedef struct SGL_Element {
    bool isHorizontal;
    SDL_Color color;
    SGL_Vector* children;
    SGL_ElementRect rect;
    uint8_t units;
    float gap;
    float padding;
    float margin;
    bool errored;
    bool is_new;
} SGL_Element;

void SGL_ElementDestroy(SGL_Element* target);
void SGL_ElementAddChild(SGL_Element* parent, SGL_Element* child);
void SGL_ElementCalculateSubrects(SGL_Element* target);
void SGL_ElementRenderSelfAndChildren(SDL_Renderer* renderer, SGL_Element* target);
void SGL_ElementRenderSelfAndChildrenDebug(SDL_Renderer* renderer, SGL_Element* target);

#ifdef SGL_PROD
    // PRODUCTION MODE
    // The production versions of functions don't use excess resources for
    // error reporting making them more efficient.
    // If SGL_Element throws an error, program behavior is decided by macros:
    // Define following macro to make the program crash on warnings
    //   SGL_TERMINATE_ON_WARNING   (TODO:)
    // Define following macro to enable error reporting
    //   SGL_LOG_ERRORS             (TODO:)

    SGL_Element* SGL_ElementNew(const char* config, ...);
    #define ELEMENT(cfg, ...) SGL_ElementNew(cfg __VA_OPT__(,) __VA_ARGS__, (SGL_Element*)NULL)

    // TODO: make this use fmt and variadic args, not fixed string
    void SGL_ElementThrow(SGL_Element* target, const char* message);

#else
    // DEBUG MODE
    // The debug versions of functions accepts additional debug information
    // (__FILE__ and __LINE__) as the first two arguments of the function.
    // If SGL_Element throws an error, the file and line are printed to stderr.

    SGL_Element* SGL_ElementNewDebug(const char* file, int line, const char* config, ...);
    #define ELEMENT(cfg, ...) SGL_ElementNewDebug(__FILE__, __LINE__, cfg __VA_OPT__(,) __VA_ARGS__, (SGL_Element*)NULL)

    // TODO: make this use fmt and variadic args, not fixed string
    void SGL_ElementThrowDebug(const char* file, int line, SGL_Element* target, const char* message);
    #define SGL_ElementThrow(target, message) SGL_ElementThrowDebug(file, line, target, message)

#endif


#endif
