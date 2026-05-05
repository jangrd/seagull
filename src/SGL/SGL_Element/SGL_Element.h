#ifndef SGL_ELEMENT_H
#define SGL_ELEMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <errno.h>

#include <SDL3/SDL.h>

typedef struct SGL_Element SGL_Element;

#include "../SGL_Log/SGL_Log.h"
#include "../SGL_Callback/SGL_Callback.h"
#include "../SGL_Theme/SGL_Theme.h"
#include "../SGL_Page/SGL_Page.h"

#define LJG_IMPLEMENTATION // TODO: i dont remember implementing this but okay?
#include "../../../external/LJG/LJG_MetaVec.h"

// TODO: duplicate of SGL_Page.h definition
typedef struct {
    size_t index;
    size_t size;
} _SGL_Page_Subtree;

typedef struct SGL_ElementRect {
    SDL_FRect outer;
    SDL_FRect border;
    SDL_FRect main;
    SDL_FRect inner;
} SGL_ElementRect;

typedef enum SGL_ElementStack {
    SGL_STACK_VERTICAL,
    SGL_STACK_HORIZONTAL
} SGL_ElementStack;

typedef struct SGL_ElementStyle {
    // TODO: free colors on element destroy
    SDL_Color* background_color;
    SDL_Color* border_color;
    SDL_Color* text_color;
    
    SGL_ElementStack stack;
    uint8_t units;
    float gap;
    float padding;
    float margin;
       float border;
} SGL_ElementStyle;

typedef struct SGL_Element {
    SGL_ElementRect rect;
    SGL_ElementStyle style;
    SGL_Callback on_click;
    bool is_new;
} SGL_Element;

typedef enum {
    SGL_TYPE_STYLE,
    SGL_TYPE_CHILD,
    SGL_TYPE_CALLBACK
} SGL_ElementArgumentType;

typedef struct SGL_ElementBaseArgument {
    SGL_ElementArgumentType type;
} SGL_ElementBaseArgument;

typedef struct SGL_ElementStyleArgument {
    SGL_ElementArgumentType type;
    SGL_ElementStyle        style;
} SGL_ElementStyleArgument;

typedef struct SGL_ElementChildArgument {
    SGL_ElementArgumentType type;
    size_t*                 children_ids; // LJG_MetaVec
    size_t                  count;
} SGL_ElementChildArgument;

typedef struct SGL_ElementCallbackArgument {
    SGL_ElementArgumentType type;
    SGL_CallbackType        callback_type;
    SGL_Callback            callback;
} SGL_ElementCallbackArgument;

#define SGL_ELEMENT_STYLE_DEFAULT    \
    .stack        = 0,                \
    .units        = 1,                \
    .gap        = 0,                \
    .padding    = 0,                \
    .margin        = 0,                \
    .border        = 0,                \
    .background_color = NULL,        \
    .border_color = NULL,            \
    .text_color = NULL

#define SGL_STYLE(...)                                                            \
    &(SGL_ElementStyleArgument) {                                                \
        .type    = SGL_TYPE_STYLE,                                                \
        .style   = (SGL_ElementStyle){ SGL_ELEMENT_STYLE_DEFAULT, __VA_ARGS__ }    \
    }

#define SGL_CHILDREN(...)                                                                \
    &(SGL_ElementChildArgument) {                                                        \
        .type           = SGL_TYPE_CHILD,                                                    \
        .children_ids   = (size_t[]){ __VA_ARGS__ },                                    \
        .count          = sizeof((size_t[]){ __VA_ARGS__ }) / sizeof(size_t)    \
    }

#define SGL_ONCLICK(func, args) \
    &(SGL_ElementCallbackArgument) { \
        .type = SGL_TYPE_CALLBACK, \
        .callback_type = SGL_CALLBACK_ONCLICK, \
        .callback = SGL_CALLBACK(func, args) \
    }
    
size_t SGL_Element_New(SGL_Page* page, ...);
bool SGL_ElementIsPointInside(SGL_Element* target, float x, float y);

#define SGL_ELEMENT(...) \
    SGL_Element_New(&(window->pages[window->current_page_index]), __VA_ARGS__, (SGL_Element*)NULL)

#endif

