#ifndef MAINENV_H
#define MAINENV_H

// ISO8583 Server message data size
/*! \brief Header size */
#define ISO8583_HEADER_SIZE 2
#define ISO8583_MTI_SIZE 2
#define ISO8583_P_BITMAP_SIZE 16
// ================================
// System environment defines
/*! \brief Main name */
#define PROJECT_NAME "Ciso8583"
/*! \brief Workdir path */
#define WORKDIR_PATH "/opt/" PROJECT_NAME
/*! \brief Log file path */
#define LOG_PATH WORKDIR_PATH "/log"
/*! \brief Log  */
#define LOG_FILENAME LOG_PATH "/" PROJECT_NAME ".log"


// ================================
/*! \brief Max communication size */
#define COMM_MAX_SIZE 2
/*! \brief Max communication size in byte */
#define COMM_RECEIVE_SIZE_MAX COMM_MAX_SIZE + 1024
// ====
#define DEFAULT_PORT 9101
#define DEFAULT_IP "127.0.0.1"
#define MAX_CLIENTS 100
#define PERSISTANCE_CONN 1

// Error codes
#define CODE_OK 0
#define CODE_ERROR_SERVER -1
#define CODE_ERROR_OPEN_FILE -2
#define CODE_ERROR_WRITE_FILE -3

// Message
struct ISO8583_MESSAGE {
    char header[ISO8583_HEADER_SIZE + 1];
    char mti[ISO8583_MTI_SIZE + 1];
    char buffer[COMM_RECEIVE_SIZE_MAX + 1];
};
#define ISO8583 struct ISO8583_MESSAGE
#endif