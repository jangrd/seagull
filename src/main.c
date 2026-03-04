#include "./SGL/SGL.h"

int main() {
    if (!SGL_Init()) return SGL_FAILURE;

	SDL_Color color_bg = (SDL_Color){ 0x1B, 0x3C, 0x5c, 0xFF };
	SDL_Color color_dark = (SDL_Color){ 0x23, 0x4C, 0x6A, 0xFF };
	SDL_Color color_light = (SDL_Color){ 0x45, 0x68, 0x82, 0xFF };
	SDL_Color color_text = (SDL_Color){ 0xD2, 0xC1, 0xB6, 0xFF };

    SGL_Window* window = SGL_WindowNew();

	// TODO: merge this with SGL_Window??
    SDL_Color window_clear_color = color_bg;
    window->root->style.color = window_clear_color;
    
    SGL_ElementAddChild(
        window->root,
        SGL_ELEMENT(
        	SGL_STYLE(
        		.padding = 200,
        		.color = color_bg,
        		.border = 2,
				.stack = SGL_STACK_HORIZONTAL
        	),
        	SGL_CHILDREN(

				SGL_ELEMENT(
					SGL_STYLE(
						.margin = 20,
						.padding = 20,
						.gap = 20,
						.color = color_dark
					),

					SGL_CHILDREN(
						SGL_ELEMENT(
							SGL_STYLE(
								.color = color_light,
								.border = 2,
								.border_color = (SDL_Color){ 0, 0, 0, 255 }
							)
						),
						SGL_ELEMENT(
							SGL_STYLE(
								.color = color_light,
								.units = 3,
								.border = 2,
								.border_color = (SDL_Color){ 0, 0, 0, 255 }
							)
						),
					)
				),
				
				SGL_ELEMENT(
					SGL_STYLE(
						.margin = 20,
						.color = color_dark
					)
				)
        	)
        )
    );
    SGL_WindowMainloop(window);
    SGL_WindowDestroy(window);
    SGL_Quit();
    return SGL_SUCCESS;
}


// SDL_Color white = { 255, 255, 255, 255 };

// SDL_Surface *text = TTF_RenderText_Blended(
//     font,
//     "ja volim doru ali u drugom fontu\0",
//     0,
//     white
// );
// SDL_FRect pos = { 0, 0, text->w, text->h} ;
// SDL_Texture *t_text = SDL_CreateTextureFromSurface(renderer, text);
// SDL_DestroySurface(text);
// text = NULL;

// TTF_CloseFont(font);
// SDL_DestroyRenderer(renderer);
// SDL_DestroyWindow(window);
