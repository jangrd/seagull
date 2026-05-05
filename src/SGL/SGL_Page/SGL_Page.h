#ifndef __SGL_PAGE_H__
#define __SGL_PAGE_H__

#include "../SGL_Theme/SGL_Theme.h"
#include "../SGL_Element/SGL_Element.h"
#include "../SGL_Index/SGL_Index.h"

typedef struct {
    SGL_Element* arena; // LJG_MetaVec
    size_t root_index; // invalid if arena==NULL
    SGL_Index* index;
    SGL_Theme theme;
} SGL_Page;

SGL_Page SGL_Page_New();
void SGL_Page_AttachUI(SGL_Page* page, SGL_Element element);
void SGL_Page_Destroy(SGL_Page* page);

void SGL_Page_SetTheme(SGL_Page* page, SGL_Theme theme);

#endif // __SGL_PAGE_H__
