#ifndef SGL_LOG_H
#define SGL_LOG_H


#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define SGL_LOG_PREFIX  "<-> "
#define LOG_FILENAME    "sgl-last-run.log"

void SGL_Log_Init();
void SGL_Log(const char* fmt, ...);
void SGL_LogVA(const char* fmt, va_list args);

#endif
