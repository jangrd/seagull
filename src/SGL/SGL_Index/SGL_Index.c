#include "SGL_Index.h"

SGL_Index* SGL_IndexNew() {
	SGL_Index* target = (SGL_Index*)malloc(sizeof(SGL_Index));
	target->count = 0;
	target->first = NULL;
	return target;
}

// returns 0 on success, 1 on failure
bool SGL_IndexInsert(SGL_Index* index, SGL_IndexNode* after, SGL_Element* element) {
	SGL_IndexNode* new_node = (SGL_IndexNode*)malloc(sizeof(SGL_IndexNode));
	if (new_node == NULL) return 1;
	new_node->element = element;

	if (after == NULL && index->count > 0) {
		new_node->next = index->first;
		index->first = new_node;
	}

	if (index->count == 0) {
		new_node->next = NULL;
		index->first = new_node;
		index->count++;
		return 0;
	} else {
		new_node->next = after->next;
		after->next = new_node;
	}
	
	index->count++;
	return 0;
}
