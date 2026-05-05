#include "SGL_Element.h"

size_t SGL_Element_New(SGL_Page* page, ...) {
    if (page == NULL) {
        SGL_Panic("SGL_Element_New(): parameter page cannot be NULL");
    }
    
    SGL_Element element;

    element.children = NULL;
    LJG_MetaVec_Init(element.children, 4);
    if (errno == ENOMEM) {
        SGL_Panic("Ran out of memory");
    }
    
    element.on_click = (SGL_Callback){NULL, NULL};

    va_list args;
    va_start(args, page);
    bool style_set = false;    
    for (void* arg = page; arg != NULL; arg = va_arg(args, void*)) {
        SGL_ElementBaseArgument* base = arg;
        switch (base->type) {
            case SGL_TYPE_STYLE:
                if (style_set) {
                    // TODO: implement proper handling
                    printf("Style set twice, not allowed behavior\n");
                }
                SGL_ElementStyleArgument* style_arg = arg;
                element.style = style_arg->style;
                style_set = true;
                break;
                
            case SGL_TYPE_CHILD:
                SGL_ElementChildArgument* child_arg = arg;
                for (size_t i = 0; i < child_arg->count; i++) {
                    SGL_ElementAddChild(&element, child_arg->children[i]);
                }
                break;
            case SGL_TYPE_CALLBACK:
                SGL_ElementCallbackArgument* callback_arg = arg;
                if (callback_arg->callback_type == SGL_CALLBACK_ONCLICK) {
                    element.on_click = callback_arg->callback;
                }
        }
        
    }
    va_end(args);

    if (!style_set) {
        element.style = (SGL_ElementStyle){ SGL_ELEMENT_STYLE_DEFAULT };
    }

    assert(element.style.gap     >= 0.0);
    assert(element.style.padding >= 0.0);
    assert(element.style.margin  >= 0.0);
    assert(element.style.border  >= 0.0);

    LJG_MetaVec_Push(page->arena, element);
    if (errno == ENOMEM) {
        SGL_Panic("Ran out of memory");
    }

    return LJG_MetaVec_Len(page->arena) - 1;
}

void SGL_Element_Destroy(SGL_Element* element) {
    if (element == NULL) {
        SGL_Log("SGL_ElementDestroy(SGL_Element* element) was passed NULL. Ignoring...");
        return;
    }
    if (element->children == NULL) return;
    LJG_MetaVec_Free(element->children);
}

bool SGL_ElementIsPointInside(SGL_Element* target, float x, float y) {
    return (
        x >= target->rect.border.x &&
        x <= (target->rect.border.x + target->rect.border.w) &&
        y >= target->rect.border.y &&
        y <= (target->rect.border.y + target->rect.border.h)
    );
}
// 
// void SGL_ElementAddChild(SGL_Element* parent, SGL_Element child) {
//     LJG_MetaVec_Push(parent->children, child);
//     if (errno == ENOMEM) {
//         SGL_Panic("SGL_Element_AddChild(): ran out of memory");
//     }
// }
