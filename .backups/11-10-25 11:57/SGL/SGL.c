#include "SGL.h"

bool SGL_Init(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: \"%s\"\n", SDL_GetError());
        return SGL_FAILURE;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF_Init failed: \"%s\"\n", SDL_GetError());
        return SGL_FAILURE;
    }

    #ifndef SGL_PROD
    SDL_Log("SGL is running in debug mode\n");
    SDL_Log("When compiling for production use -DSGL_PROD\n");
    #endif

    return SGL_SUCCESS;
}

void SGL_Quit(void) {
    TTF_Quit();
    SDL_Quit();
}
