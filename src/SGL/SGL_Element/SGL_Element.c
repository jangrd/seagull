#include "SGL_Element.h"

SGL_Element* SGL_ElementNew(void *first_arg, ...) {
	// TODO: check for first_arg being NULL

    SGL_Element* target = (SGL_Element*)malloc(sizeof(SGL_Element));
    if (target == NULL) {
        SGL_Log("SGL_ElementNew(): malloc failed");
        return NULL;
    }
    // if not set program will shit itself
    // TODO: make this better ig?

    target->errored = false;
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

void SGL_ElementDestroy(SGL_Element* target) {
    free(target);
    target = NULL;
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

void SGL_ElementCalculateSubrects(SGL_Element* parent) {
    SGL_Vector* children = parent->children;
    float total_units = 0;
    for (size_t i = 0; i < children->count; i++) {
        total_units += children->elements[i]->style.units;
    }
    float used_units = 0;
    for (size_t i = 0; i < children->count; i++) {
        SGL_Element* child = children->elements[i];
        // TODO: get rid of this if statement
        // i believe clay solves this by using
        // along and across direction instead of vertical horizontal
        if (parent->style.stack) {
            int available_width = parent->rect.inner.w - parent->style.gap * (children->count - 1);
            child->rect.outer.x = parent->rect.inner.x + available_width / total_units * used_units + parent->style.gap * i;
            child->rect.outer.y = parent->rect.inner.y;
            child->rect.outer.w = available_width / total_units * child->style.units;
            child->rect.outer.h = parent->rect.inner.h;
        }
        else {
            int available_height = parent->rect.inner.h - parent->style.gap * (children->count - 1);
            child->rect.outer.x = parent->rect.inner.x;
            child->rect.outer.y = parent->rect.inner.y + available_height / total_units * used_units + parent->style.gap * i;
            child->rect.outer.w = parent->rect.inner.w;
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

    for (size_t i = 0; i < children->count; i++) {
        SGL_ElementCalculateSubrects(children->elements[i]);
    }
}

void SGL_ElementRenderSelfAndChildren(SDL_Renderer* renderer, SGL_Element* target) {
	// draw border
	SDL_SetRenderDrawColor(
           renderer,
           target->style.border_color.r,
           target->style.border_color.g,
           target->style.border_color.b,
           target->style.border_color.a
       );
	SDL_RenderFillRect(renderer, &(target->rect.border));

   	// draw element
    SDL_SetRenderDrawColor(
        renderer,
        target->style.color.r,
        target->style.color.g,
        target->style.color.b,
        target->style.color.a
    );
    SDL_RenderFillRect(renderer, &(target->rect.main));

    // iterate for all children
    for (size_t i = 0; i < target->children->count; i++) {
        SGL_ElementRenderSelfAndChildren(renderer, target->children->elements[i]);
    }
}


void SGL_ElementThrow(SGL_Element* target, const char* fmt, ...) {
    (void)fmt;
    target->errored = true;
}

