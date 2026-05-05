#ifndef __SGL_PAGE_H__
#define __SGL_PAGE_H__

#include "../SGL_Theme/SGL_Theme.h"
#include "../SGL_Element/SGL_Element.h"

typedef struct {
    SGL_Element* arena; // LJG_MetaVec
    _SGL_Page_Subtree* tree; // LJG_MetaVec
    SGL_Theme theme;
} SGL_Page;

SGL_Page SGL_Page_New();
void SGL_Page_AttachUI(SGL_Page* page, size_t root_id);
void SGL_Page_Destroy(SGL_Page* page);

void SGL_Page_SetTheme(SGL_Page* page, SGL_Theme theme);

#endif // __SGL_PAGE_H__
