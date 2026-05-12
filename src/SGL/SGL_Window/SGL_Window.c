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
    
    size_t* mv_queue = NULL; // indicies into mv_tree
    LJG_MetaVec_Init(mv_queue, 1024);

    size_t first = 0;
    size_t depth = 0;
    LJG_MetaVec_Push(mv_queue, page->tree_root_idx);
    size_t last = LJG_MetaVec_Len(mv_queue) - 1;
    do {
        size_t level_size = last - first;
        for (size_t i = 0; i < level_size; i++) {
            _SGL_TreeNode* current_node = &page->mv_tree[mv_queue[first++]];
            SGL_Element* current_element = &page->arena[current_node->arena_idx];

            // DRAW BORDER
            if (current_element->style.border_color == NULL) {
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
                   current_element->style.border_color->r,
                   current_element->style.border_color->g,
                   current_element->style.border_color->b,
                   current_element->style.border_color->a
               );
            }
            SDL_RenderFillRect(window->renderer, &(current_element->rect.border));

            // DRAW MAIN
            if (current_element->style.background_color == NULL) {
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
                    current_element->style.background_color->r,
                    current_element->style.background_color->g,
                    current_element->style.background_color->b,
                    current_element->style.background_color->a
               );
            }
            SDL_RenderFillRect(window->renderer, &(current_element->rect.main));
            for (size_t j = current_node->first_child; j < current_node->next_sibling; j++) {
                mv_queue[last++] = j;
            }
        }
        depth++;
    } while (first != last);
    LJG_MetaVec_Free(mv_queue);
}

void SGL_Window_SetTheme(SGL_Window* window, SGL_Theme theme) {
    for (size_t i = 0; i < LJG_MetaVec_Len(window->pages); i++) {
        SGL_Page_SetTheme(&window->pages[i], theme);
    }
}
// 
// void SGL_Window_HandleMouseclick(SGL_Window* window, SDL_Event* event) {
//     SGL_Element* clicked_element = NULL;
//     SGL_Page* page = SGL_Window_GetCurrentPage(window);
//     SGL_IndexNode* node = page->index->first;
// 
//     // TODO: oktrij jel ovo moguce
//     if (node == NULL) {
//         SGL_Panic("watafak\n");
//     }
//     
//     while (node != NULL) {
//         if (SGL_ElementIsPointInside(node->element, event->button.x, event->button.y)) {           
//            clicked_element = node->element;
//         }
//         node = node->next;    
//     }
//     if (clicked_element == NULL) {
//         SGL_Log("No clicked element found, ignoring...\n");
//         return;
//     }
// 
//     int click_was_left = -1;
//     switch (event->button.button) {
//         case SDL_BUTTON_LEFT:
//             click_was_left = true;
//             break;
//         case SDL_BUTTON_RIGHT:
//             click_was_left = false;
//             break;
//     }
// 
//     switch (event->type) {
//         case SDL_EVENT_MOUSE_BUTTON_DOWN:
//             window->mouse.last_mdown = clicked_element;
//             break;
//         case SDL_EVENT_MOUSE_BUTTON_UP:
//             if (
//                 window->mouse.last_mdown == clicked_element
//                 && window->mouse.click_was_left == click_was_left
//                 && window->mouse.click_was_left == true
//             ) {
//                 if (clicked_element->on_click.function != NULL) {
//                     (*clicked_element->on_click.function)(clicked_element, clicked_element->on_click.parameters);
//                 }
//             }
//             
//             break;
//     }
// 
//     window->mouse.click_was_left = click_was_left;
// }


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
    SGL_Page_Update(SGL_Window_GetCurrentPage(window));
}

