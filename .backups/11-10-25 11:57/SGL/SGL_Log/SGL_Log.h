#ifndef SGL_LOG_H
#define SGL_LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define SGL_LOG_PREFIX "<-> "

void SGL_Log(const char* fmt, ...);

#endif
