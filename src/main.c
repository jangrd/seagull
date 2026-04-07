#include "./SGL/SGL.h"

void myFunc(SGL_Element* element, void *arg) {
	SGL_Window* window = (SGL_Window*)arg;

    SGL_UI_START

	SGL_ElementAddChild(
		element,
		SGL_ELEMENT(
			SGL_STYLE(
				.background_color = SGL_ColorHex(0xFF00FFFF),
				.border = 2
			)
		)
	);

    SGL_UI_END
	
	size_t c = 0;
	size_t sum = 0;
	SGL_Page* page = SGL_Window_GetCurrentPage(window);
	for (SGL_IndexNode* node = page->index->first; node != NULL; node = node->next) {
		sum += sizeof(*node->element) + sizeof(*node->element->children);
		c++;
	}
	printf("%zu elements taking up %zu bytes\n", c, sum);
}

int main() {
    if (!SGL_Init()) return SGL_FAILURE;

    SGL_Window* window = SGL_Window_New();

    SGL_UI_START
    SGL_Page_AttachUI(
        SGL_Window_GetCurrentPage(window),    
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


