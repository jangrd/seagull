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

#define SGL_WINDOW_DEFAULT_TITLE "seagull - Jan Grdanjski"

typedef struct SGL_Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SGL_Element* root;
    SGL_Index* index;
    SGL_MouseState mouse;
    SGL_Theme theme;
} SGL_Window;

//# ----------------
//  User Interface
SGL_Window* SGL_Window_New();
void        SGL_Window_AttachUI(SGL_Window* window, SGL_Element* root);
void        SGL_Window_Destroy(SGL_Window* window);
void        SGL_Window_Mainloop(SGL_Window* window);

void        SGL_Window_SetTheme(SGL_Window* window, SGL_Theme theme);
//# ----------------

void SGL_Window_Render(SGL_Window* window);
void SGL_Window_Update(SGL_Window* window);
void SGL_Window_HandleMouseclick(SGL_Window* window, SDL_Event* event);


#endif 
