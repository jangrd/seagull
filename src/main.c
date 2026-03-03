#include "./SGL/SGL.h"

int main() {
    if (!SGL_Init()) return SGL_FAILURE;
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color green = {0, 255, 0, 255};
    SDL_Color blue = {0, 0, 255, 255};
    SGL_Window* window = SGL_WindowNew();
    SGL_ElementAddChild(
        window->root,
        
        SGL_ELEMENT(
        	// TODO: rename color to background and make it use 0xRGBA
        	SGL_STYLE(
        		.stack = SGL_STACK_HORIZONTAL,
        		.color = red,
        		.padding = 10
        	),
        	
        	SGL_CHILDREN(
        	
        		SGL_ELEMENT(
        			SGL_STYLE(
        				.color = green,
        				.padding = -20,
        				.units = 2,
        				.border = 5,
        				.border_color = (SDL_Color){ 255, 255, 0, 255 }
        			)
        		),

       			SGL_ELEMENT(
        			SGL_STYLE(
        				.color = blue,
        				.padding = 20,
        				.border = 10,
        				.border_color = (SDL_Color){ 255, 0, 255, 255 }
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
