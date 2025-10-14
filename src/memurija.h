#ifndef MEMURIJA_H
#define MEMURIJA_H

#include <stdlib.h>
#include <stdio.h>

#define _MEMURIJA_INCREMENT 128
void** _memurija_allocs = NULL;
size_t _memurija_count = 0;
size_t _memurija_capacity = 0;

void* _memurija_malloc(const char* file, size_t line, size_t size) {
    if (_memurija_allocs == NULL) {
        _memurija_capacity = _MEMURIJA_INCREMENT;
        _memurija_allocs = (void**)malloc(sizeof(void*) * _memurija_capacity);
    }
    void* result = malloc(size);
    if (result == NULL) {
        fprintf(stderr, "<memurija.h> In file \"%s\" at line %ld: malloc failed\n", file, line);
    }
    if (result != NULL) {
        if (_memurija_count >= _memurija_capacity) {
            // realloc
        }
        _memurija_allocs[_memurija_count++] = result;
    }

    return result;
}

void* _memurija_calloc(const char* file, size_t line, size_t nmemb, size_t size) {
    // TODO
}

void* _memurija_realloc(const char* file, size_t line, void *ptr, size_t size) {
    // TODO
}


void _memurija_free(const char* file, size_t line, void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "<memurija.h> In file \"%s\" at line %ld: free failed (cannot free NULL)\n", file, line, ptr);
        return;
    }
    bool freed = false;
    for (size_t i = 0; i < _memurija_count; i++) {
        if (_memurija_allocs[i] == ptr) {
            free(ptr);
            _memurija_allocs[i] = NULL;
            freed = true;
        }
    }
    if (!freed) {
        fprintf(stderr, "<memurija.h> In file \"%s\" at line %ld: Double-free (no memory allocated at %p)\n", file, line, ptr);
    }    
}

#define malloc(size) _memurija_malloc(__FILE__, __LINE__, size)
#define calloc(nmemb, size) _memurija_calloc(__FILE__, __LINE__, nmemb, size)
#define realloc(ptr, size) _memurija_realloc(__FILE__, __LINE__, ptr, size)
#define free(ptr) _memurija_free(__FILE__, __LINE__, ptr)


void memurija_report() {
    if (_memurija_allocs == NULL) {
        printf("<memurija.h> No allocations occured yet\n");
        return;
    }
    printf("<memurija.h> memurija_report()\n");
    printf("Total allocations: %d\n", _memurija_count);
    size_t deallocations = 0;
    for (size_t i = 0; i < _memurija_count; i++) {
        if (_memurija_allocs[i] == NULL) {
            deallocations++;
            continue;
        }
        printf(" - active pointer: %p\n", _memurija_allocs[i]);
    }
    printf("Total deallocations: %d\n", deallocations);
}


#endif
