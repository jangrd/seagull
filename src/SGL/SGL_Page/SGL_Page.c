#include "./SGL_Page.h"

SGL_Page* SGL_Page_New() {
    SGL_Page* page = (SGL_Page*)malloc(sizeof(SGL_Page));
    if (page == NULL) {
        SGL_Panic("No memory available");
    }    
    page->root = SGL_ELEMENT(NULL);
    page->index = SGL_IndexNew();
    page->theme = SGL_THEME_DEFAULT;
    return page;
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

void SGL_Page_Destroy(SGL_Page* page) {
    printf("CALL\n");
    if (page == NULL) {
        SGL_Log("SGL_Page_Destroy(SGL_Page* page) was passed NULL. Ignoring...");
        return;
    }
    SGL_Index_Destroy(page->index);
    SGL_Log("page=%p page->root=%p", (void*)page, (void*)page->root);
    SGL_ElementDestroy(page->root);
    // TODO: free index
    free(page);
}
