#ifndef __SGL_INDEX_H__
#define __SGL_INDEX_H__

#include <stddef.h>
#include "../SGL_Element/SGL_Element.h"

typedef struct SGL_IndexNode {
	SGL_Element* element;
	void* next;	
} SGL_IndexNode;

typedef struct SGL_Index {
	size_t count;
	SGL_IndexNode* first;
} SGL_Index;

SGL_Index* SGL_IndexNew();
bool SGL_IndexInsert(SGL_Index* index, SGL_IndexNode* after, SGL_Element* element);


#endif
