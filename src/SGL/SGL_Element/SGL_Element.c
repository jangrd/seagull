#include "SGL_Element.h"

size_t SGL_Element_New(SGL_Page* page, ...) {
    if (page == NULL) {
        SGL_Panic("SGL_Element_New(): parameter page cannot be NULL");
    }
    
    SGL_Element element;
    element.on_click = (SGL_Callback){NULL, NULL};
    
    va_list args;
    va_start(args, page);
    bool style_set = false;
    size_t *children    = NULL;
    size_t  child_count = 0;
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
                children    = child_arg->children_ids;
                child_count = child_arg->count;
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
    // TODO: find better solution
    size_t SGL_Page_AddElement(SGL_Page* page, SGL_Element element, size_t *children, size_t child_count);
    return SGL_Page_AddElement(page, element, children, child_count);
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

