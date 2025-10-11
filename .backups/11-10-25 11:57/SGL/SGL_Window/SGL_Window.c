#include "SGL_Window.h"

SGL_Window* SGL_WindowNew(bool debug) {
    SGL_Window* target = (SGL_Window*)malloc(sizeof(SGL_Window));
    if (target == NULL) {
        fprintf(stderr, "Failed to allocate memory for SGL_Window\n");
        return NULL;
    }

    target->window = SDL_CreateWindow("title", 1280, 720, 0);
    if (target->window == NULL) {
        fprintf(stderr, "SDL_CreateWindow failed: \"%s\"\n", SDL_GetError());
        free(target);
        return NULL;
    }

    target->renderer = SDL_CreateRenderer(target->window, NULL);
    if (target->renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer failed: \"%s\"\n", SDL_GetError());
        SDL_DestroyWindow(target->window);
        free(target);
        return NULL;
    }
    SDL_SetRenderDrawBlendMode(target->renderer, SDL_BLENDMODE_BLEND);

    // TODO: handle VectorNew failure
    target->index = SGL_VectorNew(1024, 1024);

    target->root = ELEMENT("", NULL);
    target->debug = debug;
    return target;
}

void SGL_WindowDestroy(SGL_Window* target) {
    SDL_DestroyRenderer(target->renderer);
    SDL_DestroyWindow(target->window);
    free(target);
}

void SGL_RenderRect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect rect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &rect);
}

void SGL_WindowRender(SGL_Window* target) {
    int width, height;
    if(!SDL_GetRenderOutputSize(target->renderer, &width, &height)) {
        fprintf(stderr, "SDL_GetRenderOutputSize failed: \"%s\"\n", SDL_GetError());
    }
    target->root->rect.inner.x = 0;
    target->root->rect.inner.y = 0;
    target->root->rect.inner.w = width;
    target->root->rect.inner.h = height;
    SGL_ElementCalculateSubrects(target->root);
    if (target->debug) {
        SGL_ElementRenderSelfAndChildrenDebug(target->renderer, target->root);
    }
    else {
        SGL_ElementRenderSelfAndChildren(target->renderer, target->root);
    }
}

void SGL_WindowMainloop(SGL_Window* target) {
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        SGL_Window_UpdateIndexR(target->index, target->root);
        
        for (size_t i = 0; i < target->index->count; i++) {
            if (target->index->elements[i]->errored) {
                running = false;
                break;
            }
        }
        if (running == false) break;
        // SGL_ElementThrow(target->root->children->elements[0], "pokusaj throwanja");

        SDL_SetRenderDrawColor(target->renderer, 0, 0, 0, 255);
        SDL_RenderClear(target->renderer);

        SGL_WindowRender(target);
        
        //SDL_RenderTexture(target->renderer, t_text, NULL, &pos);
        SDL_RenderPresent(target->renderer);
    }
}

void SGL_Window_UpdateIndexR(SGL_Vector* index, SGL_Element* target) {
    if (target == NULL) return;

    if (target->is_new) {
        index = SGL_VectorAdd(index, target);
        target->is_new = false;
    }
    for (size_t i = 0; i < target->children->count; i++) {
        SGL_Window_UpdateIndexR(index, target->children->elements[i]);
    }
}

