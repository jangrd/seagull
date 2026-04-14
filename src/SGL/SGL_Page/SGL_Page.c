#include "./SGL_Page.h"

SGL_Page SGL_Page_New() {
    return (SGL_Page) {
        SGL_ELEMENT(NULL),
        SGL_IndexNew(),
        SGL_THEME_DEFAULT
    };
}

void SGL_Page_Destroy(SGL_Page* page) {
    if (page == NULL) {
        SGL_Log("SGL_Page_Destroy(SGL_Page* page) was passed NULL. Ignoring...");
        return;
    }
    SGL_Index_Destroy(page->index);
    SGL_ElementDestroy(page->root);
}

void SGL_Page_AttachUI(SGL_Page* page, SGL_Element* element) {
    if (page == NULL) {
        SGL_Panic("User error, page is NULL");
    }
    SGL_Page_Destroy(page);
    page->root = element;
}

void SGL_Page_SetTheme(SGL_Page* page, SGL_Theme theme) {
    if (page == NULL) {
        SGL_Log("SGL_Page_SetTheme(...) was passed NULL for page. Ignoring... ");
        return;
    }
    page->theme = theme;
}
