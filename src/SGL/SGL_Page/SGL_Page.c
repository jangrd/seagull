#include "./SGL_Page.h"

SGL_Page SGL_Page_New() {
    return (SGL_Page) {
        NULL,
        0,
        SGL_IndexNew(),
        SGL_THEME_DEFAULT
    };
}

void SGL_Page_Destroy(SGL_Page* page) {
    if (page == NULL) {
        SGL_Log("SGL_Page_Destroy(SGL_Page* page) was passed NULL. Ignoring...");
        return;
    }

    if (page->arena == NULL) return;

    size_t count = LJG_MetaVec_Len(page->arena);
    for (size_t i = 0; i < count; i++) {
        SGL_Element_Destroy(&(page->arena[i]));
    }

    LJG_MetaVec_Free(page->arena);
    SGL_Index_Destroy(page->index);
}

// TODO: remove
#define pop(stack) temp = stack[LJG_MetaVec_Len(stack)-1]; top--;


void SGL_Page_AttachUI(SGL_Page* page, SGL_Element element) {
    if (page == NULL) {
        SGL_Panic("User error, page is NULL");
    }
    SGL_Page_Destroy(page);
    
    page->arena[page->root_index] = element;
}

void SGL_Page_SetTheme(SGL_Page* page, SGL_Theme theme) {
    if (page == NULL) {
        SGL_Log("SGL_Page_SetTheme(...) was passed NULL for page. Ignoring... ");
        return;
    }
    page->theme = theme;
}
