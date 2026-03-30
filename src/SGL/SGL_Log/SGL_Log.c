#include "SGL_Log.h"

static FILE* log_file = NULL;

void SGL_Log_Init() {
log_file = fopen(SGL_LOG_FILENAME, "w");
if (log_file == NULL) {
    fprintf(stderr, "SGL_Log failed to create a log file\n");
}
}

void SGL_Print(const char* fmt, ...) {
  va_list args, args2;
  va_start(args, fmt);
  va_copy(args2, args);

  // print to stderr
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");

  // print to logfile
  if (log_file != NULL) {
      vfprintf(log_file, fmt, args2);
      fprintf(log_file, "\n");
      fflush(log_file);
  }
  va_end(args);
  va_end(args2);
  }

  void SGL_LogVA(const char* fmt, va_list args) {
  va_list args2;
  va_copy(args2, args);

  // print to stderr
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");

  // print to logfile
  if (log_file != NULL) {
      vfprintf(log_file, fmt, args2);
      fprintf(log_file, "\n");
      fflush(log_file);
  }

  va_end(args);
}
