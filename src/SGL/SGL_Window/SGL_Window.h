#ifndef SGL_WINDOW_H
#define SGL_WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../SGL_Element/SGL_Element.h"
#include "../SGL_Theme/SGL_Theme.h"
#include "../SGL_MouseState/SGL_MouseState.h"
#include "../SGL_Index/SGL_Index.h"

typedef struct SGL_Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SGL_Element* root;
    SGL_Index* index;
    SGL_WindowContext context;
    SGL_MouseState mouse;
    SGL_Theme* theme;
} SGL_Window;

SGL_Window* SGL_WindowNew();
void SGL_WindowDestroy(SGL_Window* target);

void SGL_Window_SetTheme(SGL_Window* target, SGL_Theme* theme);
void SGL_WindowRender(SGL_Window* target);
void SGL_WindowMainloop(SGL_Window* target);
void SGL_Window_UpdateAndIndexTree(SGL_Window* target);
void SGL_Window_HandleMouseclick(SGL_Window* target, SDL_Event* event);

#endif 
