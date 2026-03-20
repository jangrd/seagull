#include "./SGL/SGL.h"

int c = 0;
void myFunc(void *arg) {
	printf("%d: Hello %s\n", c++, (char*)arg);
}

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

int main() {
    if (!SGL_Init()) return SGL_FAILURE;

	printf("Element: %zu\n", sizeof(SGL_Element));

    SGL_Window* window = SGL_WindowNew();
    SGL_Theme theme = SGL_THEME_DEFAULT;
	SGL_Window_SetTheme(window, &theme);

    SGL_ElementAddChild(
        window->root,
        SGL_ELEMENT(
        	SGL_STYLE(
        		.padding = 200,
				.stack = SGL_STACK_HORIZONTAL
        	),
        	SGL_CHILDREN(

				SGL_ELEMENT(
					SGL_STYLE(
						.margin = 20,
						.padding = 20,
						.gap = 20,
						.border = 5
					),

					SGL_CHILDREN(
						SGL_ELEMENT(
							SGL_STYLE(
								.border = 2
							),
							SGL_ONCLICK(myFunc, "jan")
						),
						SGL_ELEMENT(
							SGL_STYLE(
								.units = 3,
								.border = 2
							)
						),
					)
				),
				
				SGL_ELEMENT(
					SGL_STYLE(
						.margin = 20,
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


