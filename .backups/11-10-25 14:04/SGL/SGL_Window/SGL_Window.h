#ifndef SGL_WINDOW_H
#define SGL_WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../SGL_Element/SGL_Element.h"

#define SGL_WINDOW_PROD     false
#define SGL_WINDOW_DEBUG    true

typedef struct SGL_Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SGL_Element* root;
    SGL_Vector* index;
    bool debug;
} SGL_Window;


SGL_Window* SGL_WindowNew(bool debug);
void SGL_WindowDestroy(SGL_Window* target);

void SGL_WindowRender(SGL_Window* target);
void SGL_WindowMainloop(SGL_Window* target);
void SGL_Window_UpdateIndexR(SGL_Vector* index, SGL_Element* target);

// *font = TTF_OpenFont("./fonts/Ubuntu/Ubuntu-Regular.ttf", 72);
// if (*font == NULL) {
//     SGL_Log("TTF_OpenFont failed: \"%s\"\n", SDL_GetError());
//     return SGL_FAILURE;
// }

#endif 
