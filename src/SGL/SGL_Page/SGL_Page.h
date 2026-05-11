#ifndef __SGL_PAGE_H__
#define __SGL_PAGE_H__

#include "../SGL_Theme/SGL_Theme.h"
#include "../SGL_Element/SGL_Element.h"
#include "../SGL_TreeNode/SGL_TreeNode.h"

typedef struct {
    SGL_Element* arena; // LJG_MetaVec
    _SGL_TreeNode* mv_tree;
    size_t tree_root_idxs;
    SGL_Theme theme;
} SGL_Page;

SGL_Page SGL_Page_New();
void SGL_Page_AttachUI(SGL_Page* page, size_t root_id);
void SGL_Page_Destroy(SGL_Page* page);

size_t SGL_Page_AddElement(SGL_Page *page, SGL_Element element, size_t *children, size_t child_count);

void SGL_Page_SetTheme(SGL_Page* page, SGL_Theme theme);

#endif // __SGL_PAGE_H__
