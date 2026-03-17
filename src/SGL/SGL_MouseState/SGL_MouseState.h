#ifndef __SGL_MOUSE_STATE__
#define __SGL_MOUSE_STATE__

#include "../SGL_Element/SGL_Element.h"

typedef struct SGL_MouseState {
	SGL_Element* last_mdown;
	bool click_was_left;
} SGL_MouseState;

#endif
