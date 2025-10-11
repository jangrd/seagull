#ifndef SGL_H
#define SGL_H

#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "./SGL_Window/SGL_Window.h"
#include "./SGL_Log/SGL_Log.h"

#define SGL_FAILURE 0
#define SGL_SUCCESS 1

bool SGL_Init(void);
void SGL_Quit(void);

#endif
