#include "./SGL/SGL.h"

void myFunc(void *arg) {
	printf("Hello %s\n", (char*)arg);
}

int main() {
    if (!SGL_Init()) return SGL_FAILURE;

    SGL_Window* window = SGL_WindowNew();	    
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
						.gap = 20
					),

					SGL_CHILDREN(
						SGL_ELEMENT(
							SGL_STYLE(
								.border = 2,
								.border_color = (SDL_Color){ 0, 0, 0, 255 }
							),
							SGL_ONCLICK(myFunc, "jan")
						),
						SGL_ELEMENT(
							SGL_STYLE(
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
					)
				)
        	)
        )
    );
    SGL_Theme theme = SGL_THEME_DEFAULT;
	SGL_Window_SetTheme(window, &theme);
	SGL_WindowMainloop(window);
    SGL_WindowDestroy(window);
    SGL_Quit();
    return SGL_SUCCESS;
}


