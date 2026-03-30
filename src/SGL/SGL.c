#include "SGL.h"

SDL_Color* SGL_ColorHex(uint32_t hex) {
	SDL_Color* color = (SDL_Color*)malloc(sizeof(SDL_Color));
	if (color == NULL) {
		printf("Malloc fail\n");
		return NULL;
	}
	*color = (SDL_Color) {
		.r = (hex >> 24) & 0xFF,
		.g = (hex >> 16) & 0xFF,
		.b = (hex >> 8) & 0xFF,
		.a = hex & 0xFF
	};
	return color;
}

void SGL_ExitHandler() {
    printf("TODO: SGL_ExitHandler\n");
}

bool SGL_Init(void) {
    SGL_Log_Init();
    
    atexit(SGL_ExitHandler);
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: \"%s\"\n", SDL_GetError());
        return SGL_FAILURE;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF_Init failed: \"%s\"\n", SDL_GetError());
        return SGL_FAILURE;
    }

    #ifndef SGL_PROD
    SGL_Log("SGL is running in debug mode\n\tWhen compiling for production use -DSGL_PROD");
    #endif

    return SGL_SUCCESS;
}

void SGL_Quit(void) {
    TTF_Quit();
    SDL_Quit();
}
