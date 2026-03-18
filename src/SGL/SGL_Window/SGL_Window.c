#include "SGL_Window.h"

SGL_Window* SGL_WindowNew() {
    SGL_Window* target = (SGL_Window*)malloc(sizeof(SGL_Window));
    if (target == NULL) {
        SGL_Log("Failed to allocate memory for SGL_Window\n");
        return NULL;
    }

    target->window = SDL_CreateWindow("title", 1280, 720, 0);
    if (target->window == NULL) {
        SGL_Log("SDL_CreateWindow failed: \"%s\"\n", SDL_GetError());
        free(target);
        return NULL;
    }

    target->renderer = SDL_CreateRenderer(target->window, NULL);
    if (target->renderer == NULL) {
        SGL_Log("SDL_CreateRenderer failed: \"%s\"\n", SDL_GetError());
        SDL_DestroyWindow(target->window);
        free(target);
        return NULL;
    }
    SDL_SetRenderDrawBlendMode(target->renderer, SDL_BLENDMODE_BLEND);

    target->index = SGL_IndexNew();
    if (target->index == NULL) {
        SGL_Log("SGL_WindowNew(): creation failed due to lack of memory");
        SDL_DestroyWindow(target->window);
        free(target);
        return NULL;
    }

    target->root = SGL_ELEMENT(NULL);

    return target;
}

void SGL_WindowDestroy(SGL_Window* target) {
    SDL_DestroyRenderer(target->renderer);
    SDL_DestroyWindow(target->window);
    free(target);
}

void SGL_WindowRender(SGL_Window* target) {
    int width, height;
    if(!SDL_GetRenderOutputSize(target->renderer, &width, &height)) {
        SGL_Log("SDL_GetRenderOutputSize failed: \"%s\"\n", SDL_GetError());
    }
    target->root->rect.inner.x = 0;
    target->root->rect.inner.y = 0;
    target->root->rect.inner.w = width;
    target->root->rect.inner.h = height;
    SGL_ElementCalculateSubrects(target->root);
    SGL_ElementRenderSelfAndChildren(target->renderer, target->root);
}

void SGL_Window_SetTheme(SGL_Window* target, SGL_Theme* theme) {
	target->theme = theme;
	// TODO: remove magic
	size_t queue_size = SGL_IndexCount(target->index) + 1024;
	SGL_Element** queue = (SGL_Element**)malloc(queue_size * sizeof(SGL_Element*));

    size_t first = 0;
    size_t last = 0;
	size_t depth = 0;
	queue[last++] = target->root;
	do {
		size_t level_size = last - first;
		for (size_t i = 0; i < level_size; i++) {
            SGL_Element* current = queue[first++];

			if (depth % 2 == 1) {
            	current->style.background_color = target->theme->color_dark;
			} else {
            	current->style.background_color = target->theme->color_light;
			}
       		current->style.border_color = target->theme->color_border;
       		current->style.text_color = target->theme->color_text;
 			
            for (size_t j = 0; j < current->children->count; j++) {
            	queue[last++] = current->children->elements[j];
            }
        }
        depth++;
    } while (first != last);
	free(queue);
}

void SGL_Window_HandleMouseclick(SGL_Window* target, SDL_Event* event) {
	SGL_Element* clicked_element = NULL;
	SGL_IndexNode* node = target->index->first;
	if (node == NULL) {
		printf("watafak\n");
	}
	while (node != NULL) {
   		if (SGL_ElementIsPointInside(node->element, event->button.x, event->button.y)) {   		
   			clicked_element = node->element;
   		}
		node = node->next;	
	}
	if (clicked_element == NULL) {
		printf("No clicked element found, ignoring...\n");
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
			target->mouse.last_mdown = clicked_element;
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (
				target->mouse.last_mdown == clicked_element
				&& target->mouse.click_was_left == click_was_left
				&& target->mouse.click_was_left == true
			) {
				if (clicked_element->on_click.function != NULL) {
					(*clicked_element->on_click.function)(clicked_element->on_click.parameters);
				}
			}
			
			break;
	}

	target->mouse.click_was_left = click_was_left;
}


void SGL_WindowMainloop(SGL_Window* target) {
    bool running = true;
    SDL_Event event;
    while (running) {
        SGL_Window_UpdateAndIndexTree(target);

        while (SDL_PollEvent(&event)) {
        	switch (event.type) {
        		case SDL_EVENT_QUIT:
        			running = false;
        			break;
        		case SDL_EVENT_MOUSE_BUTTON_DOWN:
        		case SDL_EVENT_MOUSE_BUTTON_UP:
        			SGL_Window_HandleMouseclick(target, &event);
        			break;
        		default:
        			break;
        	}
        }

        SDL_SetRenderDrawColor(
        	target->renderer,
          	target->theme->color_bg.r,
        	target->theme->color_bg.g,
        	target->theme->color_bg.b,
        	target->theme->color_bg.a
        );
        SDL_RenderClear(target->renderer);

        SGL_WindowRender(target);
        
        //SDL_RenderTexture(target->renderer, t_text, NULL, &pos);
        SDL_RenderPresent(target->renderer);
    }
}

void SGL_Window_UpdateAndIndexTree(SGL_Window* target) {
    if (target == NULL) return;

    size_t stack_size = 1024;
    SGL_Element** stack = (SGL_Element**)malloc(stack_size * sizeof(SGL_Element*));    if (stack == NULL) return;
    size_t top = 0;
    stack[top++] = target->root;

    if (target->index == NULL) {
    	printf("target->index == NULL\n");
    	return;
    } 
    SGL_IndexNode* current_node = target->index->first;

	while (top > 0) {
		SGL_Element* element = stack[--top];
		
		if (element == NULL) {
			printf("ELEMENT je NULL");
			return;
		}

		if (current_node != NULL) {
			if (element == current_node->element) {
				if (current_node != NULL) {
					current_node = current_node->next;
				}
				continue;
			}
		} else {
			
		}

		for (size_t i = 0; i < element->children->count; i++) {
			if (element->children->elements[i] == NULL) {
				printf("Child %zu is NULL, skipping...\n", i);
				continue;
			}
			stack[top++] = element->children->elements[i];
		}

		if (element->is_new) {
			element->style.background_color = target->theme->color_bg;
			element->style.border_color = target->theme->color_border;
			element->style.text_color = target->theme->color_text;

			if (current_node == NULL) {
				if (SGL_IndexAppend(target->index, element)) {
					printf("SGL_IndexAppend() failed\n");
					return;
				}
			} else {
				if (SGL_IndexInsert(current_node, element) == 0) {
					current_node = current_node->next;
				} else {
					// TODO: handle better?
					printf("SGL_IndexInsert() failed\n");
				}
			}
			
		}
	}
	
    free(stack);
}


// 
// void SGL_Window_UpdateAndIndexTree(SGL_Window* target) {
//     if (target == NULL) return;
//     
// 	int stack_size = 1024;
// 	SGL_Element** stack = (SGL_Element**)malloc(stack_size * sizeof(SGL_Element*));
// 	if (stack == NULL) return;
// 	
// 	int top = 0;
// 	stack[top++] = target->root;
// 
// 	SGL_IndexNode* node = target->index->first;
// 
// 	do {
// 		// TODO: chat
// 		if (top <= 0) continue;
// 	
// 		SGL_Element* current = stack[--top];
// 		if (current == NULL) {
// 			printf("nebi se trebalo desit current==NULL\n");
// 			return;
// 		}
// 
// 		if (node == NULL) {
// 			current->style.background_color = target->theme->color_bg;
// 			current->style.border_color = target->theme->color_border;
// 			current->style.text_color = target->theme->color_text;
// 
// 			if (!SGL_IndexInsert(target->index, node, current)) {
// 				printf("SGL_IndexInsert() failed\n");
// 			}
// 		}
// 		
// 		if (node->element != current) {
// 			current->style.background_color = target->theme->color_bg;
// 			current->style.border_color = target->theme->color_border;
// 			current->style.text_color = target->theme->color_text;
// 
// 			if (!SGL_IndexInsert(target->index, node, current)) {
// 				printf("SGL_IndexInsert() failed\n");
// 			}
// 	        
// 		}
// 
// 		for (size_t i = 0; i < current->children->count; i++) {
// 			if (top >= stack_size) {
// 				stack_size *= 2;
//                 SGL_Element** new_stack = realloc(stack, stack_size * sizeof(SGL_Element*));
//                 if (new_stack == NULL) {
// 					// TODO: handle better
//                     perror("realloc");
//                     free(stack);
//                     return;
//                 }
//                 stack = new_stack;
// 			}
// 			stack[top++] = current->children->elements[i];
// 		}
// 
// 		node = node->next;
// 	} while (top > 0);	
// 	free(stack);
// }
		// TODO: possibly is_new now not needed
		// if (current->is_new) {
		// 	
	 //        current->is_new = false;
		// }
