#include "./SGL_Page.h"

SGL_Page SGL_Page_New() {
    SGL_Page page;

    page.arena = NULL;
    page.tree = NULL;
    LJG_MetaVec_Init(page.arena, 16);
    LJG_MetaVec_Init(page.tree, 16);

    page.theme = SGL_THEME_DEFAULT;

    return page;
}

void SGL_Page_Destroy(SGL_Page* page) {
    if (page == NULL) {
        SGL_Log("SGL_Page_Destroy(SGL_Page* page) was passed NULL. Ignoring...");
        return;
    }

    if (page->arena != NULL) {
        LJG_MetaVec_Free(page->arena);
    }
    if (page->tree != NULL) {
        LJG_MetaVec_Free(page->tree);
    }
    
    size_t count = LJG_MetaVec_Len(page->arena);
    for (size_t i = 0; i < count; i++) {
        SGL_Element_Destroy(&(page->arena[i]));
    }

    LJG_MetaVec_Free(page->arena);
}

// TODO: remove
#define pop(stack) temp = stack[LJG_MetaVec_Len(stack)-1]; top--;


void SGL_Page_AttachUI(SGL_Page* page, size_t root_id) {
    if (page == NULL) {
        SGL_Panic("User error, page is NULL");
    }
    SGL_Page_Destroy(page);
    
    page->root_index = root_id;
}

void SGL_Page_SetTheme(SGL_Page* page, SGL_Theme theme) {
    if (page == NULL) {
        SGL_Log("SGL_Page_SetTheme(...) was passed NULL for page. Ignoring... ");
        return;
    }
    page->theme = theme;
}
