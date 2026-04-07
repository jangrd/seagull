#ifndef __SGL_INDEX_H__
#define __SGL_INDEX_H__

#include <stddef.h>
#include "../SGL_Element/SGL_Element.h"

typedef struct SGL_IndexNode {
	SGL_Element* element;
	void* next;	
} SGL_IndexNode;

typedef struct SGL_Index {
	SGL_IndexNode* first;
} SGL_Index;


SGL_Index* SGL_IndexNew();
size_t SGL_IndexCount(SGL_Index* index);
bool SGL_IndexInsert(SGL_IndexNode* after, SGL_Element* element);
bool SGL_IndexAppend(SGL_Index* index, SGL_Element* element);
void SGL_Index_Destroy(SGL_Index* index);

#endif
