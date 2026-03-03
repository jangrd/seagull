#ifndef SGL_WINDOW_H
#define SGL_WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../SGL_Element/SGL_Element.h"

typedef struct SGL_Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SGL_Element* root;
    SGL_Vector* index;
} SGL_Window;


SGL_Window* SGL_WindowNew();
void SGL_WindowDestroy(SGL_Window* target);

void SGL_WindowRender(SGL_Window* target);
void SGL_WindowMainloop(SGL_Window* target);
void SGL_Window_UpdateIndexR(SGL_Vector* index, SGL_Element* target);

#endif 
