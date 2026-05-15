#include "./SGL/SGL.h"

void myFunc(SGL_Element* element, void* arg) {
    printf("void %p", arg);
    SDL_Color test1 = (SDL_Color){255, 0, 0, 255};
    element->style.background_color = &test1;
	SGL_Log("CLICKED in myFunc()");
}

int main() {
    if (!SGL_Init()) return SGL_FAILURE;

    SGL_Window* window = SGL_Window_New();

    SDL_Color test = (SDL_Color){255, 0, 0, 255};

    SGL_UI_START
    SGL_Page_AttachUI(
        SGL_Window_GetCurrentPage(window),    
        SGL_ELEMENT(
          SGL_STYLE(
            .padding = 200,
            .background_color = &test,
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
                  SGL_STYLE(.border = 2),
                  SGL_ONCLICK(myFunc, window)
                ),
                SGL_ELEMENT(
                  SGL_STYLE(.units = 3, .border = 2)
                ),
              )
            ),
            SGL_ELEMENT(
              SGL_STYLE(.margin = 20)
            )
          )
        )
    );
    SGL_UI_END

    SGL_Window_Mainloop(window);
    
    SGL_Window_Destroy(window);
    SGL_Quit();
    return SGL_SUCCESS;
}


