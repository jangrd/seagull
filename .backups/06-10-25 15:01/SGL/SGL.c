#include "SGL.h"

bool SGL_Init(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: \"%s\"\n", SDL_GetError());
        return SGL_FAILURE;
    }

    if (!TTF_Init()) {
        fprintf(stderr, "TTF_Init failed: \"%s\"\n", SDL_GetError());
        return SGL_FAILURE;
    }

    #ifndef SGL_PROD
    printf("<-> SGL is running in debug mode\n");
    printf("    When compiling for production use -DSGL_PROD\n");
    #endif

    return SGL_SUCCESS;
}

void SGL_Quit(void) {
    TTF_Quit();
    SDL_Quit();
}
