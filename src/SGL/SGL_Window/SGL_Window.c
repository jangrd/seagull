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

    window->pages = NULL;
    LJG_MetaVec_Init(window->pages, 1);
    LJG_MetaVec_Push(window->pages, SGL_Page_New());

    SGL_Log("Vector size after push: %zu\n", LJG_MetaVec_Len(window->pages));
    window->current_page_index = 0;
    
    return window;
}

// returns NULL incase of error
SGL_Page* SGL_Window_GetCurrentPage(SGL_Window* window) {
    if (window == NULL) return NULL;
    if (window->pages == NULL) return NULL;
    return &(window->pages[window->current_page_index]);
}

void SGL_Window_Destroy(SGL_Window* window) {
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    if (window->pages != NULL) {
        LJG_MetaVec_Free(window->pages);
    }
    free(window);
}

void SGL_Window_Render(SGL_Window* window) {
    int width, height;
    if(!SDL_GetRenderOutputSize(window->renderer, &width, &height)) {
        SGL_Panic("SDL_GetRenderOutputSize failed: \"%s\"\n", SDL_GetError());
    }
    SGL_Page* page = SGL_Window_GetCurrentPage(window);
    
    page->arena[page->mv_tree->arena_idx].rect.inner.x = 0;
    page->arena[page->mv_tree->arena_idx].rect.inner.y = 0;
    page->arena[page->mv_tree->arena_idx].rect.inner.w = width;
    page->arena[page->mv_tree->arena_idx].rect.inner.h = height;
    
    SGL_Element** queue = NULL;
    LJG_MetaVec_Init(queue, 1024);

    size_t first = 0;
    size_t depth = 0;
    LJG_MetaVec_Push(queue, &(page->arena[page->root_index]));
    size_t last = LJG_MetaVec_Len(queue) - 1;
    do {
        size_t level_size = last - first;
        for (size_t i = 0; i < level_size; i++) {
            SGL_Element* current = queue[first++];

            // DRAW BORDER
            if (current->style.border_color == NULL) {
                SDL_SetRenderDrawColor(
                   window->renderer,
                   page->theme.color_border.r,
                   page->theme.color_border.g,
                   page->theme.color_border.b,
                   page->theme.color_border.a
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
                        page->theme.color_dark.r,
                        page->theme.color_dark.g,
                        page->theme.color_dark.b,
                        page->theme.color_dark.a
                    );
                } else {
                    SDL_SetRenderDrawColor(
                        window->renderer,
                        page->theme.color_light.r,
                        page->theme.color_light.g,
                        page->theme.color_light.b,
                        page->theme.color_light.a
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
            for (size_t j = 0; j < LJG_MetaVec_Len(current->children); j++) {
                queue[last++] = &(page->arena[current->children[j]]);
            }
        }
        depth++;
    } while (first != last);
    LJG_MetaVec_Free(queue);
}

void SGL_Window_SetTheme(SGL_Window* window, SGL_Theme theme) {
    for (size_t i = 0; i < LJG_MetaVec_Len(window->pages); i++) {
        SGL_Page_SetTheme(&window->pages[i], theme);
    }
}

void SGL_Window_HandleMouseclick(SGL_Window* window, SDL_Event* event) {
    SGL_Element* clicked_element = NULL;
    SGL_Page* page = SGL_Window_GetCurrentPage(window);
    SGL_IndexNode* node = page->index->first;

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

        if (LJG_MetaVec_Len(window->pages) == 0) {
            SGL_Panic("No pages exist on window");
        }

        SDL_SetRenderDrawColor(
            window->renderer,
            window->pages[0].theme.color_bg.r,
            window->pages[0].theme.color_bg.g,
            window->pages[0].theme.color_bg.b,
            window->pages[0].theme.color_bg.a
        );
        SDL_RenderClear(window->renderer);

        SGL_Window_Render(window);
        
        SDL_RenderPresent(window->renderer);
    }
}

void SGL_Window_Update(SGL_Window* window) {
    if (window == NULL) return;
    SGL_Page* page = SGL_Window_GetCurrentPage(window);

    size_t stack_size = 1024;
    SGL_Element** stack = (SGL_Element**)malloc(stack_size * sizeof(SGL_Element*));
    if (stack == NULL) return;
    size_t top = 0;
    stack[top++] = &(page->arena[page->root_index]);

    // TODO: otkrij jel potrebno
    if (page->index == NULL) {
        SGL_Panic("window->index == NULL\n");
        return;
    }
    
    SGL_IndexNode* current_node = page->index->first;
    while (top > 0) {
        SGL_Element* element = stack[--top];

        // TODO: otkrij jel potrebno
        if (element == NULL) {
            SGL_Panic("ELEMENT je NULL");
            return;
        }

        float total_units = 0;
        for (size_t i = 0; i < LJG_MetaVec_Len(element->children); i++) {
            stack[top++] = &(page->arena[element->children[i]]);
            total_units += page->arena[element->children[i]].style.units;
        }

        float used_units = 0;
        for (size_t i = 0; i < LJG_MetaVec_Len(element->children); i++) {
            SGL_Element* child = &(page->arena[element->children[i]]);
            // TODO: get rid of this if statement
            // i believe clay solves this by using
            // along and across direction instead of vertical horizontal
            if (element->style.stack) {
                int available_width = element->rect.inner.w - element->style.gap * (LJG_MetaVec_Len(element->children) - 1);
                child->rect.outer.x = element->rect.inner.x + available_width / total_units * used_units + element->style.gap * i;
                child->rect.outer.y = element->rect.inner.y;
                child->rect.outer.w = available_width / total_units * child->style.units;
                child->rect.outer.h = element->rect.inner.h;
            }
            else {
                int available_height = element->rect.inner.h - element->style.gap * (LJG_MetaVec_Len(element->children) - 1);
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
                if (SGL_IndexAppend(page->index, element)) {
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

