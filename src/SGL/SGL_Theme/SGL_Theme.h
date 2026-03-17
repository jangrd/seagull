#ifndef __SGL_THEME_H__
#define __SGL_THEME_H__
#include <SDL3/SDL.h>

typedef struct SGL_Theme {
	SDL_Color color_bg;
	SDL_Color color_dark;
	SDL_Color color_light;
	SDL_Color color_text;
	SDL_Color color_border;
} SGL_Theme;

#define SGL_THEME_DEFAULT \
	(SGL_Theme) {	\
		.color_bg = (SDL_Color){ 0x1B, 0x3C, 0x5c, 0xFF },  \
		.color_dark = (SDL_Color){ 0x23, 0x4C, 0x6A, 0xFF }, \
		.color_light = (SDL_Color){ 0x45, 0x68, 0x82, 0xFF }, \
		.color_text = (SDL_Color){ 0xD2, 0xC1, 0xB6, 0xFF }, \
		.color_border = (SDL_Color){ 0x00, 0x00, 0x00, 0xFF} \
	}

#endif
