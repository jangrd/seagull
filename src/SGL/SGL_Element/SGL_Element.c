#include "SGL_Element.h"

SGL_Element* SGL_ElementNew(void* first_arg, ...) {
    // TODO: check for first_arg being NULL

    SGL_Element* target = (SGL_Element*)malloc(sizeof(SGL_Element));
    if (target == NULL) {
        SGL_Log("SGL_ElementNew(): malloc failed");
        return NULL;
    }
    // if not set program will shit itself
    // TODO: make this better ig?

    target->is_new = true;
    // TODO: maybe find better arguments for SGL_VectorNew
    target->children = SGL_VectorNew(1024, 1024);
    if (target->children == NULL) {
        free(target);
        return NULL;
    }
    target->on_click = (SGL_Callback){NULL, NULL};

    va_list args;
    va_start(args, first_arg);
    bool style_set = false;    
    for (void* arg = first_arg; arg != NULL; arg = va_arg(args, void*)) {
        SGL_ElementBaseArgument* base = arg;
        switch (base->type) {
            case SGL_TYPE_STYLE:
                if (style_set) {
                    // TODO: implement proper handling
                    printf("Style set twice, not allowed behavior\n");
                }
                SGL_ElementStyleArgument* style_arg = arg;
                target->style = style_arg->style;
                style_set = true;
                break;
                
            case SGL_TYPE_CHILD:
                SGL_ElementChildArgument* child_arg = arg;
                for (size_t i = 0; i < child_arg->count; i++) {
                    // TODO: NULL SAFTEY HERE
                    SGL_ElementAddChild(target, child_arg->children[i]);
                }
                break;
            case SGL_TYPE_CALLBACK:
                SGL_ElementCallbackArgument* callback_arg = arg;
                if (callback_arg->callback_type == SGL_CALLBACK_ONCLICK) {
                    target->on_click = callback_arg->callback;
                }
        }
        
    }
    va_end(args);

    if (!style_set) {
        target->style = (SGL_ElementStyle){ SGL_ELEMENT_STYLE_DEFAULT };
    }

    assert(target->style.gap     >= 0.0);
    assert(target->style.padding >= 0.0);
    assert(target->style.margin  >= 0.0);
    assert(target->style.border  >= 0.0);
    
    return target;
}

void SGL_ElementDestroy(SGL_Element* element) {
    if (element == NULL) {
        SGL_Log("SGL_ElementDestroy(SGL_Element* element) was passed NULL. Ignoring...");
        return;
    }
    if (element->children == NULL) return;
    for (size_t i = 0; i < element->children->count; i++) {
        SGL_Element* child = element->children->elements[i];
        if (child == NULL) continue;
        SGL_ElementDestroy(child);
    }
    free(element);
}

bool SGL_ElementIsPointInside(SGL_Element* target, float x, float y) {
    return (
        x >= target->rect.border.x &&
        x <= (target->rect.border.x + target->rect.border.w) &&
        y >= target->rect.border.y &&
        y <= (target->rect.border.y + target->rect.border.h)
    );
}

void SGL_ElementAddChild(SGL_Element* parent, SGL_Element* child) {
    parent->children = SGL_VectorAdd(parent->children, child);
}
