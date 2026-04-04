#ifndef __SGL_PAGE_H__
#define __SGL_PAGE_H__

#include "../SGL_Element/SGL_Element.h"
#include "../SGL_Window/SGL_Window.h"

typedef struct {
    SGL_Element root;
} SGL_Page;

SGL_Page* SGL_Page_New(SGL_Window* window);

void SGL_Page_Destroy();

#endif // __SGL_PAGE_H__
