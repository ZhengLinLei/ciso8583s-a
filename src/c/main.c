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
int start_server(int port, int max_client);
int accept_server(int idSocketServer);
void conn_exit(int idSocket);


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

void conn_exit(int idSocket) {
    close(idSocket);
    log_close();
    exit(EXIT_FAILURE);
}

/*!
 * \brief:  Arguments posible
 * \param:
 *      [port] : <num> default: 9101
 *      [ip]   : <ip>  default: 127.0.0.1
 */
int main(int argc, char *argv[]) {
    int iRet;

    char ipSocket[16];
    int portSocket;
    // Server
    int idSocketServer, idSocketClient;

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

    // Start logging
    iRet = log_start(LOG_FILENAME);

    if (iRet < 0) {
        fprintf(stderr, "Error LOGOPEN\n");
        return CODE_ERROR_OPEN_FILE;
    }

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

        // Print client info
        log_info("New client connected with ID: %d", idSocketClient);

        log_info("Force disconnection: %d", FORCE_DISCONNECTION);
        // Close connection
        close(idSocketClient);
    }

    log_info("Closing");

    // Close socket
    close(idSocketServer);
    log_close();

    return 0;
}