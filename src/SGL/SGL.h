#ifndef SGL_H
#define SGL_H

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


#if defined(_MSC_VER)
  #define SGL_UI_START __pragma(warning(push))
  #define SGL_UI_END   __pragma(warning(pop))

#elif defined(__clang__)
  #define SGL_UI_START \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Woverride-init\"") \
    _Pragma("clang diagnostic ignored \"-Woverride-init-side-effects\"")
  #define SGL_UI_END \
    _Pragma("clang diagnostic pop")

#elif defined(__GNUC__)
  #define SGL_UI_START \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Woverride-init\"") \
    _Pragma("GCC diagnostic ignored \"-Woverride-init-side-effects\"")
  #define SGL_UI_END \
    _Pragma("GCC diagnostic pop")

#else
  #define SGL_UI_START
  #define SGL_UI_END
#endif


#endif
