/*!
*   \file main.c
*   \brief Main file
*   \author Zheng Lin Lei
*/
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/select.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/time.h> 
#include <unistd.h>

// Dependencies
#include "log.h"
#include "iso8583.h"
#include "dl_iso8583.h"
#include "dl_iso8583_defs_1993.h"
#include "dl_iso8583_common.h"

// ENV Defines and CONFIGURATION
#include "env.h"

// Server socket struct
#define SA struct sockaddr 

#define DEBUG_MODE 0            // Change from 0 to 1 to show debug logs                                    (default: 0)
#define WARNING_MODE 1          // Change from 0 to 1 to show warning logs during execution                 (default: 1)
/* 
Change from 0 to 1 to force disconnection with the client in debugging mode                                 (default: 0)
(The client won't receive any response, it means that it will simulate a disconnection to see how the client acts.)

In production mode, the client can lose the connection, so it's necessary to check how both parts act to prevent errors or get stuck.

¡¡¡ It always has to be 0, for production !!!
*/
#define FORCE_DISCONNECTION 0


// Functions
// Definitions in ../h/env.h
int start_server(int port, int max_client) {
    int idSocketServer;
    struct sockaddr_in address;

    // Creating socket file descriptor
    if ((idSocketServer = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_error("Error opening socket server in port %d", port);
        conn_exit(idSocketServer);
    }

    // assign IP, PORT 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(port); 

    // Binding newly created socket to given IP and verification 
    if ((bind(idSocketServer, (SA*) &address, sizeof(address))) != 0) { 
        log_error("Socket bind failed to socket %d", idSocketServer); 
        conn_exit(idSocketServer);
    }

    // Now server is ready to listen and verification
    if (listen(idSocketServer, max_client) == -1) {
        log_error("Error listening socket %d", idSocketServer);
        conn_exit(idSocketServer);
    }

    return idSocketServer;
}
int accept_server(int idSocketServer) {
    int idSocketClient;
    struct sockaddr_in address;
    socklen_t len;

    // Accept the data packet from client and verification 
    len = sizeof(address); 
    idSocketClient = accept(idSocketServer, (SA*) &address, &len); 
    if (idSocketClient < 0) { 
        log_error("Error accepting socket %d", idSocketClient);
        conn_exit(idSocketClient);
    }

    return idSocketClient;
}

int recv_server(int idSocketClient, char* buffer, int bytes) {
    int totalBytesReceived = 0;
    int bytesReceived;

    while (totalBytesReceived < bytes) {
        bytesReceived = recv(idSocketClient, buffer + totalBytesReceived, bytes - totalBytesReceived, 0);

        if (bytesReceived <= 0) {
            // Error connection or closed in the otherside
            return -1;
        }

        totalBytesReceived += bytesReceived;
    }

    return totalBytesReceived;
}

int send_server(int idSocketClient, char* text, int bytes) {
    // std send
    int bytesSent = send(idSocketClient, text, bytes, 0);

    // Check errors during sending
    if (bytesSent <= 0) {
        return -1;
    }

    return bytesSent;
}

void conn_exit(int idSocket) {
    close(idSocket);
    log_close();
    exit(EXIT_FAILURE);
}

void client_exit(int idSocket) {
    log_info("Closing client %d", idSocket);
    close(idSocket);
    exit(EXIT_FAILURE);
}

/*!
 * \brief:  Arguments posible
 * \param:
 *      [port] : <num> default: 9101
 *      [ip]   : <ip>  default: 127.0.0.1
 */
int main(int argc, char *argv[]) {
    int iRet, bypass = 0;
    pid_t pid;

    char ipSocket[16];
    int portSocket;
    // Server
    int idSocketServer, idSocketClient;
    // Message struct
    ISO8583 iso8583Message;
    memset(&iso8583Message, '\0', sizeof(iso8583Message));

    ISO8583_INTERFACE iso8583Interface;
    memset(&iso8583Interface, '\0', sizeof(iso8583Interface));

    // ==================
    // Buffer print
    char bufferPrint[COMM_RECEIVE_SIZE_MAX+ 1];
    memset(bufferPrint, '\0', sizeof(bufferPrint));
    // Buffer send
    DL_ISO8583_HANDLER isoHandler;
	DL_ISO8583_MSG     isoMsg;
	DL_UINT8           packBuf[sizeof(iso8583Interface)];
	DL_UINT16          packedSize;

    memset(packBuf, '\0', sizeof(packBuf));

    /* 
     * get ISO-8583 1993 handler 
     * Using 1993 version because it's the most used and standarisated
     */
	DL_ISO8583_DEFS_1993_GetHandler(&isoHandler);

    // Start logging
    // If argument flag -o exist use it, if not use default LOG_FILENAME
    int customLog = 0;
    int totalArgc = argc;
    for (int i = 1; i < totalArgc; i++) {
        // Check for flags
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            // Check if there is another argument after the flag
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                // Set custom log
                customLog = 1;
                iRet = log_start(argv[i + 1]);
                i++;  // Skip the next argument (the filename)

                // Remove two indexes to avoid using incorrect arguments
                argc -= 2;
            } else {
                fprintf(stderr, "Error: Flag %s requires a filename.\n", argv[i]);
                return CODE_ERROR_ARGUMENTS;
            }
        }
    }
    // If there is no custom log, use default
    if (customLog == 0)
        iRet = log_start(LOG_FILENAME);

    if (iRet < 0) {
        fprintf(stderr, "Error LOGOPEN\n");
        return CODE_ERROR_OPEN_FILE;
    }

    // Set ip and port
    memset(ipSocket, '\0', sizeof(16));
    // --
    strcpy(ipSocket, DEFAULT_IP);
    portSocket = DEFAULT_PORT;

    // Custom argv
    if (argc >= 2)
        portSocket = atoi(argv[1]);
    if (argc >= 3)
        strcpy(ipSocket, argv[2]);

    // Wake up message
    log_info("Starting socket server in port %d", portSocket);
    /*
        Starting server with parameters:
            - PORT: %d
            - MAX_CLIENTS: %d

        Any error will be needed to be tested
        The function start_server will control the error and exit the program if it's necessary
        The function will return the socket id
    */
    idSocketServer = start_server(portSocket, MAX_CLIENTS);

    // conn_exit(idSocketServer);
    // Main loop
    while (1) {
        // Accept the data packet from client and verification 
        idSocketClient = accept_server(idSocketServer);

        // Check status
        if (idSocketClient < 0) {
            log_error("Error accepting socket %d", idSocketClient);
            conn_exit(idSocketClient);
        }

        if ((pid = fork()) < 0) {
#if WARNING_MODE == 1
            log_warning("Error forking, bypassing and forcing parent to execute");
#endif
            bypass = 1;
        }

#if DEBUG_MODE == 1
        log_debug("PID %d , Bypass %d", pid, bypass);
#endif

        // Parent
        if (pid > 0 || bypass == 1) {
            // Print client info
            log_info("New client connected with ID: %d", idSocketClient);
        }

        // Child process but can be bypassed
        if(pid == 0 || bypass == 1) {
            while (1) {
                // Ejemplo: enviar un mensaje al cliente
                // const char *message = "¡Hola, cliente!\n";
                // send(idSocketClient, message, strlen(message), 0);

                // Start receiving header (Size of message)
                iRet = recv_server(idSocketClient, iso8583Message.header, ISO8583_HEADER_SIZE);
                if(iRet < 0) {
                    log_error("Client disconnected receiving HEADER");
                    client_exit(idSocketClient);
                }
                // Bytes to int conversion
                int HEADER = bytesToInt(iso8583Message.header, ISO8583_HEADER_SIZE);

                // Start receiving MTI
                iRet = recv_server(idSocketClient, iso8583Message.mti, ISO8583_MTI_SIZE);
                if(iRet < 0) {
                    log_error("Client disconnected receiving MTI");
                    client_exit(idSocketClient);
                }
                char MTI[ISO8583_MTI_SIZE*2 + 1];
                memset(MTI, '\0', sizeof(MTI));

                // Bytes to Hex String conversion
                bytesToHexString(iso8583Message.mti, ISO8583_MTI_SIZE, MTI);

                // Start receiving buffer
                // Data size = HEADER - ISO8583_MTI_SIZE
                int BUFFER_SIZE = HEADER - ISO8583_MTI_SIZE; 
                iRet = recv_server(idSocketClient, iso8583Message.buffer, BUFFER_SIZE);
                if(iRet < 0) {
                    log_error("Client disconnected receiving MESSAGE");
                    client_exit(idSocketClient);
                }
                
                // Printing connection
                log_info("==================");

#if DEBUG_MODE == 1
                // Bytes to Hex String conversion
                bytesToHexStringBeauty(iso8583Message.buffer, BUFFER_SIZE, bufferPrint);

                log_debug("Client %d, Header: %d, MTI: %s, Data: (%d|%d),", idSocketClient, HEADER, MTI, BUFFER_SIZE, iRet);
                log_debug("Message: %s", bufferPrint);
#endif

                // Check if this mti exist or it needs response
                iRet = check_mti(MTI, MTI_LIST, sizeof(MTI_LIST) / sizeof(MTI_LIST[0]));
#if DEBUG_MODE == 1
                log_debug("MTI %s, check_mti: %d", MTI, iRet);
#endif
                if(iRet >= 0) {
                    log_info("MTI %s exist in dict", MTI);
                    log_info("Packed size: %d", HEADER);


                    // Create response
                    DL_ISO8583_MSG_Init(NULL,0,&isoMsg);

                    /*
                    *   Copy struct to interface
                    *   This is necessary because the library uses a different struct to manage the message
                    *   Struct has got header[ISO8583_HEADER_SIZE + 1] and mti[ISO8583_MTI_SIZE + 1] but interface
                    *   has got header[ISO8583_HEADER_SIZE] and mti[ISO8583_MTI_SIZE]
                    */
                    memcpy(iso8583Interface.mti, iso8583Message.mti, ISO8583_MTI_SIZE);
                    memcpy(iso8583Interface.buffer, iso8583Message.buffer, BUFFER_SIZE);

#if DEBUG_MODE == 1
                    // Bytes to Hex String conversion
                    bytesToHexStringBeauty((char *)&iso8583Interface, HEADER, bufferPrint);
                    
                    log_debug("Interface: %s", bufferPrint);
#endif
                    // Get size of message
                    packedSize = sizeof(packBuf); // packBufPtr pointing ---> iso8583Interface
                    log_info("Requesting message with MTI %s", MTI);
                    /* unpack ISO message according to documentation */
	                (void)DL_ISO8583_MSG_Unpack(&isoHandler, (DL_UINT8 *)&iso8583Interface, packedSize, &isoMsg);

                    /* output ISO message content */
                    DL_ISO8583_MSG_Dump(LOG_PTR, NULL, &isoHandler, &isoMsg);

                    /*
                    * Operate with ISO8583 message
                    */
                    fillWithMTI(MTI, &isoMsg);
                    // memset(&isoMsg.field[2], '\0', sizeof(DL_ISO8583_MSG_FIELD));

                    log_info("Responding message with MTI %s", isoMsg.field[0].ptr);
                    DL_ISO8583_MSG_Dump(LOG_PTR, NULL, &isoHandler, &isoMsg);

                    // Packing message
                    memset(packBuf, '\0', sizeof(packBuf));
                    memset(&iso8583Interface, '\0', sizeof(iso8583Interface));
                    (void)DL_ISO8583_MSG_Pack(&isoHandler, &isoMsg, (DL_UINT8 *)&iso8583Interface, &packedSize);

                    log_info("Packed size: %d", packedSize);
                    // Add header = packedSize 2bytes in packBuf
                    intTo2Bytes(packedSize, packBuf, ISO8583_HEADER_SIZE);
                    // Add buffer = iso8583Interface in packbuf
                    memcpy(packBuf + ISO8583_HEADER_SIZE, &iso8583Interface, packedSize);

                    /* free ISO message */
                    DL_ISO8583_MSG_Free(&isoMsg);

#if DEBUG_MODE == 1
                    // Bytes to Hex String conversion
                    bytesToHexStringBeauty((char *)packBuf, packedSize + ISO8583_HEADER_SIZE, bufferPrint);
                    log_debug("Response: %s", bufferPrint);
#endif
                    // Response
                    send_server(idSocketClient, packBuf, packedSize + ISO8583_HEADER_SIZE);
                } else {
                    // Ommit if mti doesn't exist
                    log_info("MTI %s doesn't exist in dict, ommiting connection", MTI);
                }

                // End operation
                log_info("==================");

#if PERSISTANCE_CONN != 1
                // Close connection
                client_exit(idSocketClient);
                break;
#endif
            }
        }
    }


    // Close socket
    close(idSocketServer);
    log_close();

    return 0;
}