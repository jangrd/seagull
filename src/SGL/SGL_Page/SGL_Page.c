#include "./SGL_Page.h"

SGL_Page SGL_Page_New() {
    SGL_Page page;

    page.arena = NULL;
    page.mv_tree = NULL;
    LJG_MetaVec_Init(page.arena, 16);
    LJG_MetaVec_Init(page.mv_tree, 16);

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
    if (page->mv_tree != NULL) {
        LJG_MetaVec_Free(page->mv_tree);
    }
}

// TODO: remove
#define pop(stack) temp = stack[LJG_MetaVec_Len(stack)-1]; top--;


// void SGL_Page_AttachUI(SGL_Page* page, size_t root_id) {
//     if (page == NULL) {
//         SGL_Panic("User error, page is NULL");
//     }
//     SGL_Page_Destroy(page);
// }

size_t SGL_Page_AddElement(SGL_Page *page, SGL_Element element, size_t *children, size_t child_count) {
    LJG_MetaVec_Push(page->arena, element);
    if (errno == ENOMEM)
        SGL_Panic("Ran out of memory");

    size_t arena_i = LJG_MetaVec_Len(page->arena) - 1;

    _SGL_TreeNode node = {
        .arena_idx    = arena_i,
        .parent       = SGL_TREENODE_NULL,
        .first_child  = SGL_TREENODE_NULL,
        .next_sibling = SGL_TREENODE_NULL,
    };
    LJG_MetaVec_Push(page->mv_tree, node);
    if (errno == ENOMEM)
        SGL_Panic("Ran out of memory");

    size_t tree_i = LJG_MetaVec_Len(page->mv_tree) - 1;

    for (size_t c = 0; c < child_count; c++) {
        size_t ci = children[c];
        page->mv_tree[ci].parent = tree_i;

        size_t *slot = &page->mv_tree[tree_i].first_child;
        while (*slot != SGL_TREENODE_NULL)
            slot = &page->mv_tree[*slot].next_sibling;
        *slot = ci;
    }

    return tree_i;
}

void SGL_Page_SetTheme(SGL_Page* page, SGL_Theme theme) {
    if (page == NULL) {
        SGL_Log("SGL_Page_SetTheme(...) was passed NULL for page. Ignoring... ");
        return;
    }
    page->theme = theme;
}
