#include <stdarg.h>

#ifndef LOG_H
#define LOG_H


int log_start(char* filename);
void log_close(void);
void log_format(const char* tag, const char* message, va_list args);
void log_error(const char* message, ...);
void log_info(const char* message, ...);
void log_debug(const char* message, ...);


// Define filename
extern FILE* LOG_PTR;
extern char* LOG_NAME;
#endif