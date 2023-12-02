#include <stdarg.h>

#ifndef LOG_H
#define LOG_H

/*!
* \brief Initializes logging by opening a file with the specified filename.
* \param filename (char*) - A pointer to a string representing the name of the log file.
* \return (int) - Returns 0 if the file was opened successfully, otherwise returns -1.
*/
int log_start(char* filename);

/*!
* \brief Closes the log file.
*/
void log_close(void);

/*!
* \brief Writes a message to the log file.
* \param tag (char*) - A pointer to a string representing the tag of the message.
* \param message (char*) - A pointer to a string representing the message.
* \param args (va_list) - A list of arguments to be formatted into the message.
*/
void log_format(const char* tag, const char* message, va_list args);

/*!
* \brief Writes an error message to the log file.
* \param message (char*) - A pointer to a string representing the message.
* \param ... - A list of arguments to be formatted into the message.
*/
void log_error(const char* message, ...);

/*!
* \brief Writes an info message to the log file.
* \param message (char*) - A pointer to a string representing the message.
* \param ... - A list of arguments to be formatted into the message.
*/
void log_info(const char* message, ...);

/*!
* \brief Writes a warning message to the log file.
* \param message (char*) - A pointer to a string representing the message.
* \param ... - A list of arguments to be formatted into the message.
*/
void log_warning(const char* message, ...);

/*!
* \brief Writes a debug message to the log file.
* \param message (char*) - A pointer to a string representing the message.
* \param ... - A list of arguments to be formatted into the message.
*/
void log_debug(const char* message, ...);


// Define filename

/*!
* \brief Global variable to save File pointer.
*/
extern FILE* LOG_PTR;

/*!
* \brief Global variable to save Filename.
*/
extern char* LOG_NAME;
#endif