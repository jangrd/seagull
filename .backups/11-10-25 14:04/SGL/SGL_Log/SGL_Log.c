#include "SGL_Log.h"

#ifdef SGL_PROD
	// PRODUCTION MODE
	// empty functions
	// TODO: when i add logging to file ask user
	//       if they want to keep it in prod
	void SGL_Log_Init() {}
	// (void)variable - marks it as used so compilers shut up
	void SGL_Log(const char* fmt, ...) { (void)fmt; }
	void SGL_LogVA(const char* fmt, va_list args) { (void)fmt; (void)args; }
#else
	static FILE* log_file = NULL;

	void SGL_Log_Init() {
		log_file = fopen(LOG_FILENAME, "w");
		if (log_file == NULL) {
			fprintf(stderr, "%sSGL_Log failed to create a log file\n", SGL_LOG_PREFIX);
		}
	}

	// DEBUG MODE
	void SGL_Log(const char* fmt, ...) {
		va_list args, args2;
		va_start(args, fmt);
		va_copy(args2, args);

		// print to stderr
		fprintf(stderr, "%s\t", SGL_LOG_PREFIX);
		vfprintf(stderr, fmt, args);
		fprintf(stderr, "\n");
		
		// print to logfile
		if (log_file != NULL) {
			fprintf(log_file, "%s\t", SGL_LOG_PREFIX);
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
		fprintf(stderr, "%s\t", SGL_LOG_PREFIX);
		vfprintf(stderr, fmt, args);
		fprintf(stderr, "\n");
		
		// print to logfile
		if (log_file != NULL) {
			fprintf(log_file, "%s\t", SGL_LOG_PREFIX);
			vfprintf(log_file, fmt, args2);
			fprintf(log_file, "\n");
			fflush(log_file);
		}

		va_end(args2);
	}

#endif
