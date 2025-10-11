#include "SGL_Lib.h"

const char* fetchNextByDelim(const char* src, char* result, char delimiter) {
    // TODO remove 128 byte limit
    // pass pointer to buffer and length
    char buffer[128] = "\0";
    size_t buffer_length = 0;
    bool running = true;
    while (running) {
        if (*src == ' ' || *src == '\t') {
            src++;
            continue;
        }
        if (*src == '\0') {
            memset(result, 0, 128);
            break;
        }
        if (*src == delimiter) {
            buffer[buffer_length] = '\0';
            src++;
            strcpy(result, buffer);
            break;
        }
        buffer[buffer_length++] = *src;
        src++;
    }
    return src;
}
