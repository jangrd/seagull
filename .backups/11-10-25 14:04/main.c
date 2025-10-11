#include "./SGL/SGL.h"

int main() {
    if (!SGL_Init()) return SGL_FAILURE;
    SGL_Window* window = SGL_WindowNew(SGL_WINDOW_PROD);
	for (size_t i = 0; i < 500; i++) {
		SGL_ElementAddChild(
			window->root,
			ELEMENT("background: #FF00FF;")
		);
	}
    SGL_ElementAddChild(
        window->root,
        ELEMENT(
            "stack: vertical;"
            "gap: 100;"
            "background: #000000;"
			"padding: 2;",
            ELEMENT(
                "gap: 50;"
                "background: #FFFFFF;",
                ELEMENT("background: #FF0000;"),
                ELEMENT("background: #00FF00; opacity: 64;")
            ),
            ELEMENT(
                "background: #FF00FF;"
                "opacity: 255;"
                "gap: 50;"
				"margin: 10;",
                ELEMENT("background: #0000FF;"),
                ELEMENT("background: #F0F00F;")
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
