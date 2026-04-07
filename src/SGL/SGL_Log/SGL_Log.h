#ifndef SGL_LOG_H
#define SGL_LOG_H


#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define SGL_LOG_PREFIX   "<-> "
#define SGL_PANIC_PREFIX "<!> "
#define SGL_LOG_FILENAME "sgl.log"

void SGL_Log_Init();
void SGL_Print(const char* fmt, ...);
void SGL_LogVA(const char* fmt, va_list args);

#define SGL_Log(fmt, ...)                    \
    fprintf(stderr, "%s", SGL_LOG_PREFIX); \
    SGL_Print(fmt, ##__VA_ARGS__)

#ifndef SGL_PROD
#define SGL_Panic(fmt, ...)                         \
    do {                                            \
        fprintf(stderr, "%s", SGL_PANIC_PREFIX);    \
        SGL_Print(                                  \
            "%s:%d\n\t" fmt,                        \
            __FILE__, __LINE__,                     \
            ##__VA_ARGS__                           \
        );                                          \
        exit(EXIT_FAILURE);                         \
    } while(0)
#else
#define SGL_Panic(fmt, ...)                         \
    do {                                            \
        fprintf(stderr, "%s", SGL_PANIC_PREFIX);    \
        SGL_Print(fmt, ##__VA_ARGS__);              \
        exit(EXIT_FAILURE);                         \
    } while(0)
#endif


#endif
