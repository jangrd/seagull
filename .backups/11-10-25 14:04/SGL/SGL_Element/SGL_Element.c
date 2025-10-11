#include "SGL_Element.h"

#ifdef SGL_PROD
SGL_Element* SGL_ElementNew(const char* config, ...) {
#else
SGL_Element* SGL_ElementNewDebug(const char* file, int line, const char* config, ...) {
#endif
    SGL_Element* target = (SGL_Element*)malloc(sizeof(SGL_Element));
    if (target == NULL) {
        SGL_Log("SGL_ElementNew(): malloc failed");
        return NULL;
    }
    // if not set program will shit itself
    // TODO: make this better ig?
    target->units = 1;
    target->isHorizontal = 1;
    target->margin = 0;
    target->padding = 0;
    target->color.r = 0;
    target->color.g = 0;
    target->color.b = 0;
    target->color.a = 255;
    target->gap = 0;
    target->errored = false;
    target->is_new = true;
    char key[128];
    char value[128];
    if (strcmp(config, "") != 0) {
        while (true) {
            config = fetchNextByDelim(config, key, ':');
            config = fetchNextByDelim(config, value, ';');
            if (key[0] == '\0') {
                break;
            }
            if (value[0] == '\0') {
                SGL_ElementThrow(target, "Missing value for key '%s'", key);
                break;
            }
            if (strcmp(key, "stack") == 0) {
                if (strcmp(value, "vertical") == 0) {
                    target->isHorizontal = 0;
                }
                else if (strcmp(value, "horizontal") == 0) {
                    target->isHorizontal = 1;
                }
            }
            else if (strcmp(key, "units") == 0) {
                char* ptr = value;
                bool valid = true;
                while (*ptr) {
                    if (*ptr < '0' || *ptr > '9') {
                        valid = false;
                        SGL_ElementThrow(target, "Key '%s' expected int, got '%s'", key, value);
                        break;
                    }
                    ptr++;
                }
                if (valid) {
                    target->units = atoi(value);
                }
            }
            else if (strcmp(key, "background") == 0) {
                if (value[0] != '#' || strlen(value) != 7) {
                    SGL_ElementThrow(target, "Key '%s' expected hex color, got '%s'", key, value);
                    break;
                }
                char* ptr = value + 1;
                uint8_t color_buffer = 0;
                uint8_t* next_color = &(target->color.r);
                while (*ptr) {
                    if (*ptr >= '0' && *ptr <= '9') {
                        color_buffer *= 16;
                        color_buffer += *ptr - '0';
                    }
                    else if (*ptr >= 'A' && *ptr <= 'F') {
                        color_buffer *= 16; 
                        color_buffer += *ptr - 'A' + 10;
                    }
                    else {
                        SGL_ElementThrow(target, "Key '%s' expected hex color, got '%s'", key, value);
                        break;
                    }
                    if ((ptr - value) % 2 == 0) {
                        *(next_color) = color_buffer;
                        next_color++;
                        color_buffer = 0;
                    }
                    ptr++;
                }
            }
            else if (strcmp(key, "margin") == 0) {
                char* ptr = value;
                bool valid = true;
                while (*ptr) {
                    if (*ptr < '0' || *ptr > '9') {
                        valid = false;
                        SGL_ElementThrow(target, "Key '%s' expected int, got '%s'", key, value);
                        break;
                    }
                    ptr++;
                }
                if (valid) {
                    target->margin = atoi(value);
                }
            }
            else if (strcmp(key, "padding") == 0) {
                char* ptr = value;
                bool valid = true;
                while (*ptr) {
                    if (*ptr < '0' || *ptr > '9') {
                        valid = false;
                        SGL_ElementThrow(target, "Key '%s' expected int, got '%s'", key, value);
                        break;
                    }
                    ptr++;
                }
                if (valid) {
                    target->padding = atoi(value);
                }
            }
            else if (strcmp(key, "gap") == 0) {
                char* ptr = value;
                bool valid = true;
                while (*ptr) {
                    if (*ptr < '0' || *ptr > '9') {
                        valid = false;
                        SGL_ElementThrow(target, "Key '%s' expected int, got '%s'", key, value);
                        break;
                    }
                    ptr++;
                }
                if (valid) {
                    target->gap = atoi(value);
                }
            }
            else if (strcmp(key, "opacity") == 0) {
                char* ptr = value;
                bool valid = true;
                while (*ptr) {
                    if (*ptr < '0' || *ptr > '9') {
                        valid = false;
                        SGL_ElementThrow(target, "Key '%s' expected int [0-255], got '%s'", key, value);
                        break;
                    }
                    ptr++;
                }
                if (valid) {
                    int opacity = atoi(value);
                    if (opacity >= 0 && opacity <= 255) {
                        target->color.a = opacity;
                    } else {
                        SGL_ElementThrow(target, "Key '%s' expected int [0-255], got '%s'", key, value);
                    }
                }
            }
            else {
                SGL_ElementThrow(target, "Unknown key: '%s'", key);
            }
        }
    }

    target->children = SGL_VectorNew(1024, 1024);
    if (target->children == NULL) {
        free(target);
        return NULL;
    }
    va_list args;
    va_start(args, config);
    SGL_Element* arg = va_arg(args, SGL_Element*);
    while (arg != NULL) {
        SGL_ElementAddChild(target, arg);
        arg = va_arg(args, SGL_Element*);
    }
    va_end(args);
    return target;
}

void SGL_ElementDestroy(SGL_Element* target) {
    free(target);
    target = NULL;
}

void SGL_ElementAddChild(SGL_Element* parent, SGL_Element* child) {
    parent->children = SGL_VectorAdd(parent->children, child);
}

void SGL_ElementCalculateSubrects(SGL_Element* parent) {
    SGL_Vector* children = parent->children;
    float total_units = 0;
    for (size_t i = 0; i < children->count; i++) {
        total_units += children->elements[i]->units;
    }
    float used_units = 0;
    for (size_t i = 0; i < children->count; i++) {
        SGL_Element* child = children->elements[i];
        if (parent->isHorizontal) {
            int available_width = parent->rect.inner.w - parent->gap * (children->count - 1);
            child->rect.outer.x = parent->rect.inner.x + available_width / total_units * used_units + parent->gap * i;
            child->rect.outer.y = parent->rect.inner.y;
            child->rect.outer.w = available_width / total_units * child->units;
            child->rect.outer.h = parent->rect.inner.h;
        }
        else {
            int available_height = parent->rect.inner.h - parent->gap * (children->count - 1);
            child->rect.outer.x = parent->rect.inner.x;
            child->rect.outer.y = parent->rect.inner.y + available_height / total_units * used_units + parent->gap * i;
            child->rect.outer.w = parent->rect.inner.w;
            child->rect.outer.h = available_height / total_units * child->units;
        }
        child->rect.border.x = child->rect.outer.x + child->margin;
        child->rect.border.y = child->rect.outer.y + child->margin;
        child->rect.border.w = child->rect.outer.w - 2 * child->margin;
        child->rect.border.h = child->rect.outer.h - 2 * child->margin;

        child->rect.inner.x = child->rect.border.x + child->padding;
        child->rect.inner.y = child->rect.border.y + child->padding;
        child->rect.inner.w = child->rect.border.w - 2 * child->padding;
        child->rect.inner.h = child->rect.border.h - 2 * child->padding;

        used_units += child->units;
    } 

    for (size_t i = 0; i < children->count; i++) {
        SGL_ElementCalculateSubrects(children->elements[i]);
    }
}

void SGL_ElementRenderSelfAndChildren(SDL_Renderer* renderer, SGL_Element* target) {
    SDL_SetRenderDrawColor(
        renderer,
        target->color.r,
        target->color.g,
        target->color.b,
        target->color.a
    );
    SDL_RenderFillRect(renderer, &(target->rect.border));
    // TODO: make this add to a queue instead of recursion
    for (size_t i = 0; i < target->children->count; i++) {
        SGL_ElementRenderSelfAndChildren(renderer, target->children->elements[i]);
    }
}

void SGL_ElementRenderSelfAndChildrenDebug(SDL_Renderer* renderer, SGL_Element* target) {
    for (size_t i = 0; i < target->children->count; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 221, 135, 255);
        SDL_RenderFillRect(renderer, &(target->children->elements[i]->rect.outer));

        SDL_SetRenderDrawColor(renderer, 135, 255, 187, 255);
        SDL_RenderFillRect(renderer, &(target->children->elements[i]->rect.border));

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderRect(renderer, &(target->children->elements[i]->rect.border));

        SDL_SetRenderDrawColor(
            renderer,
            target->color.r,
            target->color.g,
            target->color.b,
            target->color.a
        );
        SDL_RenderFillRect(renderer, &(target->children->elements[i]->rect.inner));

    }
    for (size_t i = 0; i < target->children->count; i++) {
        SGL_ElementRenderSelfAndChildrenDebug(renderer, target->children->elements[i]);
    }
}

#ifdef SGL_PROD

    void SGL_ElementThrow(SGL_Element* target, const char* fmt, ...) {
        (void)fmt;
        target->errored = true;
    }

#else

    void SGL_ElementThrowDebug(const char* file, int line, SGL_Element* target, const char* fmt, ...) {
        target->errored = true;
        va_list args;
		va_start(args, fmt);
        SGL_Log("SGL_Element threw an error in \"%s\" at line %d:", file, line);
        SGL_LogVA(fmt, args);
        va_end(args);
    }

#endif

