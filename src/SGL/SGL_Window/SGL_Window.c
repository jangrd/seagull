#include "SGL_Window.h"

// TODO: window set title

SGL_Window* SGL_Window_New() {
    SGL_Window* window = (SGL_Window*)malloc(sizeof(SGL_Window));
    if (window == NULL) {
        SGL_Panic("No memory available");
        // return NULL;
    }

    window->window = SDL_CreateWindow(SGL_WINDOW_DEFAULT_TITLE, 1280, 720, 0);
    if (window->window == NULL) {
        free(window);
        SGL_Panic("SDL_CreateWindow failed: \"%s\"\n", SDL_GetError());
        // return NULL;
    }

    window->renderer = SDL_CreateRenderer(window->window, NULL);
    if (window->renderer == NULL) {
        SDL_DestroyWindow(window->window);
        free(window);
        SGL_Panic("SDL_CreateRenderer failed: \"%s\"\n", SDL_GetError());
        // return NULL;
    }
    SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);


	SGL_Window_SetTheme(window, SGL_THEME_DEFAULT);
    window->root = SGL_ELEMENT(NULL);
    window->index = SGL_IndexNew();
    // under the assumption that failure points of SGL_IndexNew
    // are handled by SGL_Panic as it is at the time im writing this
    // then the following code is unneeded
    // 
    // if (window->index == NULL) {
    //     SDL_DestroyWindow(window->window);
    //     free(window);
    //     SGL_Panic("SGL_WindowNew(): creation failed due to lack of memory");
    //     return NULL;
    // }

    return window;
}

void SGL_Window_AttachUI(SGL_Window* window, SGL_Element* root) {
    if (root == NULL) {
        SGL_Panic("Passed UI root is NULL\n");
    }
    window->root = root;
}

void SGL_Window_Destroy(SGL_Window* window) {
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    free(window);
}

void SGL_Window_Render(SGL_Window* window) {
    int width, height;
    if(!SDL_GetRenderOutputSize(window->renderer, &width, &height)) {
        SGL_Panic("SDL_GetRenderOutputSize failed: \"%s\"\n", SDL_GetError());
    }
    window->root->rect.inner.x = 0;
    window->root->rect.inner.y = 0;
    window->root->rect.inner.w = width;
    window->root->rect.inner.h = height;
    
    size_t queue_size = SGL_IndexCount(window->index) + 1024;
    SGL_Element** queue = (SGL_Element**)malloc(queue_size * sizeof(SGL_Element*));

    size_t first = 0;
    size_t last = 0;
    size_t depth = 0;
    queue[last++] = window->root;
    do {
        size_t level_size = last - first;
        for (size_t i = 0; i < level_size; i++) {
            SGL_Element* current = queue[first++];

            // DRAW BORDER
            if (current->style.border_color == NULL) {
                SDL_SetRenderDrawColor(
                   window->renderer,
                   window->theme.color_border.r,
                   window->theme.color_border.g,
                   window->theme.color_border.b,
                   window->theme.color_border.a
               );
            } else {
                SDL_SetRenderDrawColor(
                   window->renderer,
                   current->style.border_color->r,
                   current->style.border_color->g,
                   current->style.border_color->b,
                   current->style.border_color->a
               );
            }
            SDL_RenderFillRect(window->renderer, &(current->rect.border));

            // DRAW MAIN
            if (current->style.background_color == NULL) {
                if (depth % 2 == 1) {
                    SDL_SetRenderDrawColor(
                        window->renderer,
                        window->theme.color_dark.r,
                        window->theme.color_dark.g,
                        window->theme.color_dark.b,
                        window->theme.color_dark.a
                    );
                } else {
                    SDL_SetRenderDrawColor(
                        window->renderer,
                        window->theme.color_light.r,
                        window->theme.color_light.g,
                        window->theme.color_light.b,
                        window->theme.color_light.a
                    );
                }
            } else {
                SDL_SetRenderDrawColor(
                    window->renderer,
                    current->style.background_color->r,
                    current->style.background_color->g,
                    current->style.background_color->b,
                    current->style.background_color->a
               );
            }
            SDL_RenderFillRect(window->renderer, &(current->rect.main));
                          
            for (size_t j = 0; j < current->children->count; j++) {
                queue[last++] = current->children->elements[j];
            }
        }
        depth++;
    } while (first != last);
    free(queue);
}

void SGL_Window_SetTheme(SGL_Window* window, SGL_Theme theme) {
    window->theme = theme;
}

void SGL_Window_HandleMouseclick(SGL_Window* window, SDL_Event* event) {
    SGL_Element* clicked_element = NULL;
    SGL_IndexNode* node = window->index->first;

    // TODO: oktrij jel ovo moguce
    if (node == NULL) {
        SGL_Panic("watafak\n");
    }
    
    while (node != NULL) {
        if (SGL_ElementIsPointInside(node->element, event->button.x, event->button.y)) {           
           clicked_element = node->element;
        }
        node = node->next;    
    }
    if (clicked_element == NULL) {
        SGL_Log("No clicked element found, ignoring...\n");
        return;
    }

    int click_was_left = -1;
    switch (event->button.button) {
        case SDL_BUTTON_LEFT:
            click_was_left = true;
            break;
        case SDL_BUTTON_RIGHT:
            click_was_left = false;
            break;
    }

    switch (event->type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            window->mouse.last_mdown = clicked_element;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (
                window->mouse.last_mdown == clicked_element
                && window->mouse.click_was_left == click_was_left
                && window->mouse.click_was_left == true
            ) {
                if (clicked_element->on_click.function != NULL) {
                    (*clicked_element->on_click.function)(clicked_element, clicked_element->on_click.parameters);
                }
            }
            
            break;
    }

    window->mouse.click_was_left = click_was_left;
}


void SGL_Window_Mainloop(SGL_Window* window) {
    bool running = true;
    SDL_Event event;
    while (running) {
        SGL_Window_Update(window);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    SGL_Window_HandleMouseclick(window, &event);
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(
            window->renderer,
            window->theme.color_bg.r,
            window->theme.color_bg.g,
            window->theme.color_bg.b,
            window->theme.color_bg.a
        );
        SDL_RenderClear(window->renderer);

        SGL_Window_Render(window);
        
        SDL_RenderPresent(window->renderer);
    }
}

void SGL_Window_Update(SGL_Window* window) {
    if (window == NULL) return;

    size_t stack_size = 1024;
    SGL_Element** stack = (SGL_Element**)malloc(stack_size * sizeof(SGL_Element*));
    if (stack == NULL) return;
    size_t top = 0;
    stack[top++] = window->root;

    // TODO: otkrij jel potrebno
    if (window->index == NULL) {
        SGL_Panic("window->index == NULL\n");
        return;
    }
    
    SGL_IndexNode* current_node = window->index->first;
    while (top > 0) {
        SGL_Element* element = stack[--top];

        // TODO: otkrij jel potrebno
        if (element == NULL) {
            SGL_Panic("ELEMENT je NULL");
            return;
        }

        float total_units = 0;
        for (size_t i = 0; i < element->children->count; i++) {
            if (element->children->elements[i] == NULL) {
                printf("Child %zu is NULL, skipping...\n", i);
                continue;
            }
            stack[top++] = element->children->elements[i];
            total_units += element->children->elements[i]->style.units;
        }

        float used_units = 0;
        for (size_t i = 0; i < element->children->count; i++) {
            SGL_Element* child = element->children->elements[i];
            // TODO: get rid of this if statement
            // i believe clay solves this by using
            // along and across direction instead of vertical horizontal
            if (element->style.stack) {
                int available_width = element->rect.inner.w - element->style.gap * (element->children->count - 1);
                child->rect.outer.x = element->rect.inner.x + available_width / total_units * used_units + element->style.gap * i;
                child->rect.outer.y = element->rect.inner.y;
                child->rect.outer.w = available_width / total_units * child->style.units;
                child->rect.outer.h = element->rect.inner.h;
            }
            else {
                int available_height = element->rect.inner.h - element->style.gap * (element->children->count - 1);
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

        if (element->is_new) {
            if (current_node == NULL) {
                if (SGL_IndexAppend(window->index, element)) {
                    SGL_Panic("Ran out of memory\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                if (SGL_IndexInsert(current_node, element) == 0) {
                    current_node = current_node->next;
                } else {
                    SGL_Panic("Ran out of memory\n");
                    exit(EXIT_FAILURE);
                }
            }
            element->is_new = false;            
        }

        if (current_node != NULL) {
            if (element == current_node->element) {
                if (current_node != NULL) {
                    current_node = current_node->next;
                }
            }
        }
    }    
    free(stack);
}

