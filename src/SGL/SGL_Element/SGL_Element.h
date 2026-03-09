#ifndef SGL_ELEMENT_H
#define SGL_ELEMENT_H

#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "../SGL_Vector/SGL_Vector.h"
#include "../SGL_Lib/SGL_Lib.h"
#include "../SGL_Log/SGL_Log.h"
#include "../SGL_Callback/SGL_Callback.h"

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
	SGL_ElementStack stack;
	// TODO: RGBA as int
    SDL_Color color;
    // positive integer
    uint8_t units;
    // 3/ non-negative
    float gap;
    // non-negative
    float padding;
    // non-negative
    float margin;
    // non-negative
	float border;
	// TODO: RGBA as int
	SDL_Color border_color;
} SGL_ElementStyle;

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
	SGL_ElementStyle style;
} SGL_ElementStyleArgument;

typedef struct SGL_ElementChildArgument {
	SGL_ElementArgumentType	type;
	SGL_Element**			children;
	size_t					count;
} SGL_ElementChildArgument;

typedef struct SGL_ElementCallbackArgument {
	SGL_ElementArgumentType type;
	SGL_CallbackType callback_type;
	SGL_Callback callback;
} SGL_ElementCallbackArgument;

#define SGL_ELEMENT_STYLE_DEFAULT								\
	.stack		= 0,											\
	.color		= (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255},	\
	.units		= 1,											\
	.gap		= 0,											\
	.padding	= 0,											\
	.margin		= 0,											\
	.border		= 0,											\
	.border_color = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255}


#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverride-init-side-effects"
#endif

#define SGL_STYLE(...)															\
	&(SGL_ElementStyleArgument) {												\
		.type	= SGL_TYPE_STYLE,												\
		.style	= (SGL_ElementStyle){ SGL_ELEMENT_STYLE_DEFAULT, __VA_ARGS__ }	\
	}	

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

// TODO: chat said to use SGL_Element*[]
//		 understand why and see if it has to be that way
#define SGL_CHILDREN(...)																\
	&(SGL_ElementChildArgument) {														\
		.type		= SGL_TYPE_CHILD,													\
		.children	= (SGL_Element*[]){ __VA_ARGS__ },									\
		.count		= sizeof((SGL_Element*[]){ __VA_ARGS__ }) / sizeof(SGL_Element*)	\
	}

#define SGL_ONCLICK(func, args) \
	&(SGL_ElementCallbackArgument) { \
		.type = SGL_TYPE_CALLBACK, \
		.callback_type = SGL_CALLBACK_ONCLICK, \
		.callback = SGL_CALLBACK(func, args) \
	}

typedef struct SGL_Element {
    SGL_ElementRect rect;
	SGL_ElementStyle style;
    SGL_Vector* children;
    SGL_Callback on_click;
    // TODO: turn these bools into unified state
    bool errored;
    bool is_new;
} SGL_Element;

void SGL_ElementDestroy(SGL_Element* target);
bool SGL_ElementIsPointInside(SGL_Element* target, float x, float y);
void SGL_ElementAddChild(SGL_Element* parent, SGL_Element* child);
void SGL_ElementCalculateSubrects(SGL_Element* target);
void SGL_ElementRenderSelfAndChildren(SDL_Renderer* renderer, SGL_Element* target);

SGL_Element* SGL_ElementNew(void *first_arg, ...);
#define SGL_ELEMENT(...) SGL_ElementNew(__VA_ARGS__, (SGL_Element*)NULL)

void SGL_ElementThrow(SGL_Element* target, const char* fmt, ...);

#endif
