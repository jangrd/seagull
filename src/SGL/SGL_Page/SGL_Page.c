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

    page->tree_root_idx = tree_i;
    
    return tree_i;
}

void SGL_Page_Update(SGL_Page* page) {
    if (page == NULL) {
        SGL_Log("SGL_Page_Update() was passed NULL. ignoring...");
        return;
    }
    
    // TODO: IMPLEMENT PROPER STACK, THIS IS IMPROPER!!!
    size_t* mv_stack; // indicies into page->mv_tree
    LJG_MetaVec_Init(mv_stack, 1024);
    size_t top = 0;
    mv_stack[top++] = page->tree_root_idx;
    
    while (top > 0) {
        _SGL_TreeNode* node = &page->mv_tree[mv_stack[--top]];
        SGL_Element* element = &page->arena[node->arena_idx];

        float total_units = 0;
        for (size_t i = node->first_child; i < node->next_sibling; i++) {
            mv_stack[top++] = i;
            total_units += element->style.units;
        }

        float used_units = 0;
        for (size_t i = node->first_child; i < node->next_sibling; i++) {
            SGL_Element* child = &(page->arena[i]);
            // TODO: get rid of this if statement
            // i believe clay solves this by using
            // along and across direction instead of vertical horizontal
            if (element->style.stack) {
                int available_width = element->rect.inner.w - element->style.gap * (node->next_sibling - node->first_child - 1);
                child->rect.outer.x = element->rect.inner.x + available_width / total_units * used_units + element->style.gap * i;
                child->rect.outer.y = element->rect.inner.y;
                child->rect.outer.w = available_width / total_units * child->style.units;
                child->rect.outer.h = element->rect.inner.h;
            }
            else {
                int available_height = element->rect.inner.h - element->style.gap * (node->next_sibling - node->first_child - 1);
                child->rect.outer.x = element->rect.inner.x;
                child->rect.outer.y = element->rect.inner.y + available_height / total_units * used_units + element->style.gap * i;
                child->rect.outer.w = element->rect.inner.w;
                child->rect.outer.h = available_height / total_units * child->style.units;
            }
            child->rect.border.x = child->rect.outer.x + child->style.margin;
            child->rect.border.y = child->rect.outer.y + child->style.margin;
            child->rect.border.w = child->rect.outer.w - 2 * child->style.margin;
            child->rect.border.h = child->rect.outer.h - 2 * child->style.margin;
    
            child->rect.main.x = child->rect.border.x + child->style.border;
            child->rect.main.y = child->rect.border.y + child->style.border;
            child->rect.main.w = child->rect.border.w - 2 * child->style.border;
            child->rect.main.h = child->rect.border.h - 2 * child->style.border;
    
            child->rect.inner.x = child->rect.main.x + child->style.padding;
            child->rect.inner.y = child->rect.main.y + child->style.padding;
            child->rect.inner.w = child->rect.main.w - 2 * child->style.padding;
            child->rect.inner.h = child->rect.main.h - 2 * child->style.padding;
    
            used_units += child->style.units;
        }
    }
}

void SGL_Page_SetTheme(SGL_Page* page, SGL_Theme theme) {
    if (page == NULL) {
        SGL_Log("SGL_Page_SetTheme(...) was passed NULL for page. Ignoring... ");
        return;
    }
    page->theme = theme;
}
