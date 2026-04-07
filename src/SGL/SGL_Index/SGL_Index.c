#include "SGL_Index.h"

// function failpoints: malloc
// handled by: SGL_Panic
SGL_Index* SGL_IndexNew() {
    SGL_Index* target = (SGL_Index*)malloc(sizeof(SGL_Index));
    if (target == NULL) {
        SGL_Panic("No memory available");
    }
    target->first = NULL;
    return target;
}

size_t SGL_IndexCount(SGL_Index* index) {
    size_t count = 0;
    SGL_IndexNode* node = index->first;
    while (node != NULL) {
        node = node->next;
        count++;
    }
    return count;
}

// returns 0 on success, 1 on failure
bool SGL_IndexInsert(SGL_IndexNode* after, SGL_Element* element) {
    SGL_IndexNode* new_node = (SGL_IndexNode*)malloc(sizeof(SGL_IndexNode));
    if (new_node == NULL) return 1;
    new_node->element = element;

    if (after == NULL) {
        printf("after cant be NULL\n");
        return 1;
    }

    new_node->next = after->next;
    after->next = new_node;
    
    return 0;
}

bool SGL_IndexAppend(SGL_Index* index, SGL_Element* element) {
    SGL_IndexNode* new_node = (SGL_IndexNode*)malloc(sizeof(SGL_IndexNode));
    if (new_node == NULL) return 1;
    new_node->element = element;
    new_node->next = NULL;

    SGL_IndexNode* node = index->first;
    if (node == NULL) {
        index->first = new_node;
        return 0;
    }
    
    while (node->next != NULL) {
        node = node->next;
    }

    return SGL_IndexInsert(node, element);
}

void SGL_Index_Destroy(SGL_Index* index) {
    if (index == NULL) {
        SGL_Log("SGL_Index_Destroy(SGL_Index* index) was passed NULL. Ignoring...");
        return;
    }
    SGL_IndexNode* node = index->first;
    SGL_IndexNode* next = NULL;
    while (node->next != NULL) {
        next = node->next;
        free(node);
        node = next;
    }
}
