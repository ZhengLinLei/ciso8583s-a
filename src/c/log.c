#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#include "log.h"

FILE* LOG_PTR;
char* LOG_NAME;

int log_start(char* filename) {
    LOG_PTR = fopen(filename, "a");
    if (LOG_PTR == NULL) 
        return -1;

    LOG_NAME = filename;
    log_info("Starting log for %s", filename);

    return 0;
}
void log_close() {
    log_info("Closing log for %s", LOG_NAME);
    fclose(LOG_PTR);
}
void log_format(const char* tag, const char* message, va_list args) {
    time_t now;
    time(&now);
    char * date = ctime(&now);
    date[strlen(date) - 1] = '\0';
    fprintf(LOG_PTR, "%s [%s] ", date, tag);
    vfprintf(LOG_PTR, message, args);
    fprintf(LOG_PTR, "\n");
    fflush(LOG_PTR);
}

void log_error(const char* message, ...) {  
    va_list args;
    va_start(args, message);
    log_format("error", message, args);
    va_end(args);
}

void log_info(const char* message, ...) {   
    va_list args;
    va_start(args, message);
    log_format("info", message, args); 
    va_end(args);
}


void log_debug(const char* message, ...) {  
    va_list args;
    va_start(args, message);
    log_format("debug", message, args);
    va_end(args);
}