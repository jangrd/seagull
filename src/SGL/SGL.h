#ifndef SGL_H
#define SGL_H

#if defined(clang) || defined(GNUC)
    #define SGL_PRAGMA_PUSH_WOVERRIDEINIT() \
        _Pragma("GCC diagnostic push")        \
        _Pragma("GCC diagnostic ignored \"-Woverride-init\"")
    #define SGL_PRAGMA_POP_WOVERRIDEINIT()  \
        _Pragma("GCC diagnostic pop")
#else
    #define SGL_PRAGMA_PUSH_WOVERRIDEINIT()
    #define SGL_PRAGMA_POP_WOVERRIDEINIT()
#endif

#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "./SGL_Window/SGL_Window.h"
#include "./SGL_Log/SGL_Log.h"
#include "./SGL_Theme/SGL_Theme.h"

#define SGL_FAILURE 0
#define SGL_SUCCESS 1
// TODO: find a better place for this
SDL_Color* SGL_ColorHex(uint32_t hex);

void SGL_ExitHandler();

bool SGL_Init(void);
void SGL_Quit(void);

#endif
