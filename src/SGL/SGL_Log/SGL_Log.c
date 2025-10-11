#include "SGL_Log.h"

void SGL_Log(const char* fmt, ...) {
	va_list args;
    va_start(args, fmt);
	fprintf(stderr, SGL_LOG_PREFIX);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
    va_end(args);
}

