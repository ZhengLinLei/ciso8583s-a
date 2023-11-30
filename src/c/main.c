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

#define DEBUG_MODE 0            // Cambia de 0 a 1 para mostrarte logs de debug                             (default: 0)
#define WARNING_MODE 1          // Cambia de 0 a 1 para mostrar los warning en caso de error de petición    (default: 1)
/* 
Cambia de 0 a 1 para forzar desconexión con el cliente en ambito de debugging                               (default: 0)
(El cliente no recibe respuesta, es decir que simula desconexión para ver como el cliente actua.)

En casos de producción el cliente puede perder conexión red,
por lo que hay que comprobar como actua ambas partes para prevenir que lance errores o se atasque.

¡¡¡ Siempre tiene que estar en 0, para producción !!!!
*/
#define FORCE_DISCONNECTION 0



/*!
 * \brief:  Arguments posible
 * \param:
 *      [port] : <num> default: 9101
 *      [ip]   : <ip>  default: 127.0.0.1
 */
int main(int argc, char *argv[]) {

    int iRet;

    char ipSocket[16];
    int idSocket, portSocket, idClient;
    struct sockaddr_in address, client;

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

    
    log_info("Starting socket server in port %d", portSocket);
    
    // Creating socket file descriptor
    if ((idSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_error("Error opening socket server in port %d", portSocket);
        log_close();
        exit(EXIT_FAILURE);
    }

    // assign IP, PORT 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(portSocket); 

    // Binding newly created socket to given IP and verification 
    if ((bind(idSocket, (SA*)&address, sizeof(address))) != 0) { 
        log_error("Socket bind failed to socket %d", idSocket); 
        log_close();
        exit(EXIT_FAILURE); 
    }


    log_info("Closing");


    // Close socket
    close(idSocket);

    return 0;
}


// /* ======================================================================== */
// /* Example filename data format                                             */
// /* --                                                                       */
// /* yyyymmddhhiissAAAAAAPPPCCCCC                                             */
// /* AAAAAA = Ticket   number                                                 */
// /* PPP    = Pos      number                                                 */
// /* CCCCC  = Commerce number                                                 */
// /* -------------------------------------------------------------------------*/
// /*  Example request data format                                             */
// /*  --                                                                      */
// /*  BBBBBB|yyyymmddhhiissAAAAAAPPPCCCCC|SIGNATURE_DATA_HEX|                 */
// /* -------------------------------------------------------------------------*/
// /* Must include Control Character |                                         */
// /* ======================================================================== */


// static int SignatureService_readConfig();
// static int SignatureService_readConfigDB();
// static int SignatureService_connectDB(void);
// static int SignatureService_disconnectDB(void);
// static int SignatureMain_closeConnection(int);
// static int SignatureMain_checkMessageEnd(char *);
// static int SignatureMain_checkMessageSize(char *, int);
// static int SignatureMain_checkMessageContent(char *, int, int *);
// static int SignatureMain_writeFile(char *, char *, char *);
// static int SignatureMain_commReceiveSocket(int, char *, int, int, int);
// static int SignatureMain_commSendSocket(int, char *);


// /*
//  * Private global variables definitions
//  */
// /*! \brief Buffer de datos recividos */
// char DataReceivedPos[COMM_RECEIVE_SIZE_MAX + 1];
// /*! \brief Buffer de datos enviados */
// char DataRespondPos[POS_DATA_HEADER_SIZE + 1];

// char PosDataSize[POS_DATA_CHECK_SIZE + 1];
// // 2023110910300011111100088888
// char PosDataHeader[POS_DATA_HEADER_SIZE + 1];
// char PosDataEmptyHeader[POS_DATA_HEADER_SIZE + 1];
// char PosDataBody[COMM_MAX_SIZE];

// /*
//  * Functions
//  */
// /*! \brief Read default configuration */
// static int SignatureService_readConfig(void) {
// 	base_pathBuilder_struct cfg_path;
// 	base_pathBuilder_translate(&cfg_path, BASE_PATHDEFINE_PARAM_PATH);
// 	if (conf_general_load(cfg_path.value)) {
// 		SPLOG_ERROR("Error loading Global Configuration from \"%s\"", cfg_path.value);
// 		return 1;
// 	}
// 	return 0;
// }

// /*! \brief Reading configuration from DB */
// static int SignatureService_readConfigDB(void) {
// 	if (ConfigContainer::loadFromDB() != 0) {
// 		SPLOG_EMERG("Error loading configuration from DB");
// 		return 1;
// 	}
// 	SPLOG_NOTICE("Configuration loaded");
	
// 	return 0;
// }

// /*! \brief Connection with DB */
// static int SignatureService_connectDB(void) {
// 	SPLOG_NOTICE("Opening the database");
// 	SPLOG_NOTICE("Initializing access to the database");
// 	if (DBA_st_init_database() == -1) {
//         SPLOG_ERROR("ERROR initializing access to the database");
// 		return -1;
// 	}
// 	SPLOG_NOTICE("Access to database initialized");

// 	return 0;
// }

// /*! \brief Disconnection with DB */
// static int SignatureService_disconnectDB(void) {
// 	SPLOG_NOTICE("Closing the database");
// 	DBA_st_close_database();
// 	return 0;
// }

// /* =================================== */
// /* Main functions                      */
// /* =================================== */
// /*! \brief Close connection with client socket id */
// static int SignatureMain_closeConnection(int idsocket) {
//     struct linger ling;
//     size_t val;
//     val = sizeof(ling);
//     ling.l_onoff = 1;
//     ling.l_linger = 10;
//     /* fcntl (idsocket,F_SETFL,fcntl(idsocket,F_GETFL) | O_NONBLOCK); */
//     setsockopt(idsocket, SOL_SOCKET, SO_LINGER, &ling, val);
//     /*shutdown (idsocket,2); */
//     close(idsocket);
//     return 0;
// }
// /*! \brief Comprueba si el mensaje enviado por el cliente esta completo por EOM o ha sido corrompiedo o perdido */
// static int SignatureMain_checkMessageEnd(char *Buffer) {
//     if(Buffer[strlen(Buffer) - 1] == SIGNATURE_DATA_SPLIT_CHAR) {
//         return 0;
//     }
//     return 1;
// }
// /*! \brief Comprueba si el mensaje enviado por el cliente esta completo por TAMAÑO o ha sido corrompiedo o perdido */
// static int SignatureMain_checkMessageSize(char *Buffer, int size) {
//     if(strlen(Buffer) == size) {
//         return 0;
//     }
//     return 1;
// }
// /*! \brief Comprueba si existe caracter no valido o numero impar de bytes */
// static int SignatureMain_checkMessageContent(char *Buffer, int size, int *output) {
//     int i = 0;
// 	// Remove "new line"or space at end of string if exist
// 	while (Buffer[size - 1] == '\n' || Buffer[size - 1] == ' ') {
// 		Buffer[size - 1] = '\0';
// 		size--;
// 	}

//     while (1) {
//         // Finish
//         if (i >= size)
//             break;

//         if ((Buffer[i] < '0' || Buffer[i] > '9') && 
//             (Buffer[i] < 'A' || Buffer[i] > 'F')) {
//             *output = (int) Buffer[i];
//             return 4; 
//         }

//         i++;
//     }

//     if (i % 2 != 0) {
//         *output = i;
//         return 5;
//     }
    
//     return 0;
// }
// /*! \brief Abre el directorio de firmas y escribe dentro del archivo la firma con sus metadatos en el nombre */
// static int SignatureMain_writeFile(char *path, char *filename, char *content) {
//     char *inter = "/";

//     // File path
//     char *filedir = new char[strlen(path)+strlen(filename)+2];

//     // Verifica si se asignó memoria correctamente
//     if (filedir == NULL) {
//         SPLOG_ERROR("Error asignando memoria para %s/%s", path, filename);
//         return 1;
//     }

//     memset(filedir, '\0', sizeof(filedir));
//     strcpy(filedir, path);
//     strcat(filedir, inter);
//     strcat(filedir, filename);

//     FILE *fptr;

//     // Open directory + filename
//     fptr = fopen(filedir, "w");

//     if(fptr == NULL) {
//         SPLOG_ERROR("Error abriendo fichero para %s", filedir);
//         return 1;           
//     }

//     fprintf(fptr, "%s", content);
//     fclose(fptr);

//     return 0;
// }
// /*! \brief Recibe datos del cliente hasta parar */
// static int SignatureMain_commReceiveSocket(int idsocket, char *Buffer, int Long, int Intentos, int Lap) {
// 	int Recibidos, iRet;
// 	int real_iSize, repetitions = 0;
// 	fd_set listo;
// 	struct timeval timeout, t0, t1;

// 	do {
// 		if (idsocket < 0) {
// 			comm_errno = EINVAL;
// 			SPLOG_WARN("Error receiving from socket:%d (%s)", COMM_ERROR_SOCKET_RECEIVE,
// 					   "Wrong parameter.");

// 			return COMM_ERROR_SOCKET_RECEIVE;
// 		}

// 		/* Obtenemos el instante actual */
// 		gettimeofday(&t0, (struct timezone *)0);
// 		repetitions = 0;
// 		do {
// 			repetitions++;
// 			FD_ZERO(&listo);
// 			FD_SET(idsocket, &listo);
// 			/* En funcion del numero de iteraciones del bucle calculamos el timeout */
// 			if (repetitions == 1) {
// 				/* Asignamos el timeout inicial */
// 				timeout.tv_sec = Lap;
// 				timeout.tv_usec = 0;
// 			}
// 			else {
// 				/* Obtenemos el insntante de ejecucion del bucle */
// 				gettimeofday(&t1, (struct timezone *)0);
// 				/* Calculamos el timeout en funcion de los instantes */
// 				timeout.tv_sec = Lap - (t1.tv_sec - t0.tv_sec);
// 				timeout.tv_usec = 0;
// 			}
// 			real_iSize = 0;
// 			errno = 0;
// 			iRet = select(idsocket + 1, &listo, 0, 0, &timeout);
// 		} while (iRet == -1 && errno == EINTR);

// 		if (iRet < 0) {
// 			comm_errno = errno;
// 			SPLOG_WARN("Error receiving from socket [select] Return:%d errno:%d (%s)",
// 					   COMM_ERROR_SOCKET_RECEIVE, errno, strerror(errno));
// 			return (COMM_ERROR_SOCKET_RECEIVE);
// 		}
// 		if (iRet == 0) {  /* time out */ 
// 			Intentos--;
// 		}
// 		else {
// 			do {
// 				Recibidos =
// 					recv(idsocket, (char *)Buffer + real_iSize,
// 						 Long - real_iSize, 0);

// 				real_iSize += Recibidos;

//                 if(SignatureMain_checkMessageEnd(Buffer) == 0) {
//                     return 1;
//                 }

//                 if(real_iSize >= Long) {
//                     // Size exceded
//                     return -2;
//                 }

//                 // Control data
//                 switch (Recibidos) {
//                     case 0:
//                         // The server stopped receiving successfully
//                         return 0;
//                         break;
                
//                     case COMM_ERROR_SOCKET_RECEIVE:
//                         return COMM_ERROR_SOCKET_RECEIVE;
//                         break;
//                 }

//                 if(Recibidos < 0) {
//                     // If the server stop receiving due 
//                     // close connection or end of message must be checked after
//                     SPLOG_ALARM("Client closed the connection, try again");
//                     return -1;
//                 }

// 				if (real_iSize == Long) {
// 					comm_errno = 0;
// 					return 0;
// 				}
// 			} while (real_iSize < Long);

// 			comm_errno = errno;

// 			return (COMM_ERROR_SOCKET_RECEIVE);
// 		}
// 	} while (Intentos > 0);
// 	comm_errno = errno;
// 	SPLOG_WARN("Error:%d (%s)", COMM_ERROR_SOCKET_RECEIVE,
// 			   "Retries exceeded.");
// 	return (COMM_ERROR_SOCKET_RECEIVE);
// }
// /*! \brief Enviar respuesta de vuelta al cliente */
// static int SignatureMain_commSendSocket(int idsocket, char *content) {
// #if DEBUG_MODE
//     SPLOG_DEBUG("Response: %s", content);
// #endif
//     return comm_sendSocket(
//                 idsocket, content, POS_DATA_HEADER_SIZE, 1,
//                 ConfigContainer::SERVER_POS_CONNECTION_TIMEOUT);
// }


// /* ======================================================================== */
// /* Main Proccess                                                            */
// /* ======================================================================== */
// int main(void) {

//     // Environment to LOG status
//     int iRet, res;
//     base_pathBuilder_struct path;

//     int idSignatureSocket;
//     int idSocketAccepted;
//     string ipSocket;
//     int portSocket;

//     int signatureReceive;
//     int signatureRespond;

//     int processInitialization;
    
//     base_pathBuilder_translate(&path, LOG_SIGNATURE_SERVER);
//     iRet = SPLOG_OPEN(path.value);
// 	if (iRet < 0) {
// 		fprintf(stderr, "Error LOGOPEN\n");
// 		return BASE_PROCESSMANAGER_ERROR_OPEN_LOG;
// 	}

//     // Wake up message
//     SPLOG_NOTICE("-------------------------------------------------------");
// 	SPLOG_NOTICE("Starting solverpay-signature-server process");

//     if (SignatureService_readConfig()) {
// 		return BASE_PROCESSMANAGER_ERROR_LOAD_CFG;
// 	}

// 	if (SignatureService_connectDB()) {
//         return BASE_PROCESSMANAGER_ERROR_CHECK_DB;
// 	}

// 	if (SignatureService_readConfigDB()) {
// 		SignatureService_disconnectDB();
// 		return BASE_PROCESSMANAGER_ERROR_OPEN_DB;
// 	}

//     // base_process_type process;

//     // base_process_init(&process);
//     // process.processTitle = "solverpay-signature-server";
//     // process.logPath = LOG_SIGNATURE_SERVER;

//     // processInitialization = base_process_start(&process);
//     // if (processInitialization) {
//     //     return processInitialization;
//     // }

//     ipSocket = ConfigContainer::SIGNATURE_IP_LISTEN;
//     portSocket = ConfigContainer::SIGNATURE_PORT_LISTEN;


//     // Init socket listener
//     SPLOG_NOTICE("Server initializing...");
//     idSignatureSocket =
//       comm_openSocket(ipSocket.c_str(),
//                       portSocket);

//     if (idSignatureSocket == COMM_ERROR_OPEN_SOCKET || idSignatureSocket == COMM_ERROR_BIND || idSignatureSocket == COMM_ERROR_LISTEN) {
//         SPLOG_EMERG("Error while opening server socket.");
//         SignatureService_disconnectDB();
//         log_Transactions_close();
//         SPLOG_CLOSE();
//         return BASE_PROCESSMANAGER_ERROR_SERVER_SOCKET;
//     }

//     SPLOG_NOTICE("Server listening in PORT: %d with ID: %d", portSocket, idSignatureSocket);

//     // Set error answer
//     memset(PosDataEmptyHeader, '\0', POS_DATA_HEADER_SIZE+1); // Full NULL
//     memset(PosDataEmptyHeader, '0', POS_DATA_HEADER_SIZE);

//     /* ============================== */
//     /*     Request listener code      */
//     /*                                */
//     /* ============================== */

//     do {
//         // Check status
//         idSocketAccepted = comm_AcceptRequest(idSignatureSocket);

// #if DEBUG_MODE
//     SPLOG_DEBUG("Socket ID: %d", idSocketAccepted);
// #endif

//         // Skip error
//         if (idSocketAccepted == COMM_ERROR_ACCEPT_PROCESS) {
//             continue;
//         }

//         if (idSocketAccepted < 0) {
//             SPLOG_ERROR("Error accepting socket in PORT: %d with ID: %d", portSocket, idSignatureSocket);
//             sleep(1);
//         } else {
//             // Reset
//             memset(DataReceivedPos, '\0', COMM_MAX_SIZE);
//             memset(DataRespondPos, '\0', POS_DATA_HEADER_SIZE);
//             // Receive
//             // Using own function (If the client don't send nothing stop receiving)
//             signatureReceive = SignatureMain_commReceiveSocket(
//                                                 idSocketAccepted, DataReceivedPos, COMM_MAX_SIZE, 1,
//                                                 ConfigContainer::SERVER_POS_CONNECTION_TIMEOUT);

// #if DEBUG_MODE
//     SPLOG_DEBUG("Content %s", DataReceivedPos);
// #endif

//             // Check status
//             // 0000000000000000000000000000 = Error Server
//             // 0000000000000000000000000001 = Error EOM
//             // 0000000000000000000000000002 = Error SIZE
//             // 0000000000000000000000000003 = Error SIZE EXCEDED
//             // 0000000000000000000000000004 = Error INVALID CHAR
//             // 0000000000000000000000000005 = Error ODD SIZE
//             switch (signatureReceive) {
//                 case COMM_ERROR_SOCKET_RECEIVE:
//                     SPLOG_ERROR("Error reading request in PORT: %d with ID: %d", portSocket, idSignatureSocket);
//                     SignatureMain_closeConnection(idSocketAccepted);
//                     SPLOG_CLOSE();

//                     // Server error = 0
//                     PosDataEmptyHeader[POS_DATA_HEADER_SIZE-1] = '0';
//                     SignatureMain_commSendSocket(idSocketAccepted, PosDataEmptyHeader);

//                     return BASE_PROCESSMANAGER_ERROR_SERVER_SOCKET;
//                     break;

//                 case -2:
//                     // Response size exceded
// #if WARNING_MODE
//     SPLOG_DEBUG("Error SIZE EXCEDED: size_limit %dKb", COMM_MAX_K);
// #endif
//                     // Size limit error = 3
//                     PosDataEmptyHeader[POS_DATA_HEADER_SIZE-1] = '3';
//                     SignatureMain_commSendSocket(idSocketAccepted, PosDataEmptyHeader);

//                     continue;
//                     break;

//                 case -1:
//                     // Ommit this proccess
//                     // Client closed connextion
//                     SignatureMain_closeConnection(idSocketAccepted);

//                     continue;
//                     break;
//                 case 0:
//                     // Check if the message is completed after stopped receiving message data
//                     // Server stopped receiving data
//                     if (SignatureMain_checkMessageEnd(DataReceivedPos)) {
// #if WARNING_MODE
//     SPLOG_DEBUG("Error EOM: char %c", DataReceivedPos[strlen(DataReceivedPos) - 1]);
// #endif
//                         // EOM error = 1
//                         PosDataEmptyHeader[POS_DATA_HEADER_SIZE-1] = '1';
//                         SignatureMain_commSendSocket(idSocketAccepted, PosDataEmptyHeader);

//                         // Ommit this proccess
//                         SignatureMain_closeConnection(idSocketAccepted);

//                         continue;
//                     }
//                     break;
//             }

//             memset(PosDataSize, '\0', POS_DATA_CHECK_SIZE);
//             memset(PosDataHeader, '\0', POS_DATA_HEADER_SIZE);
//             memset(PosDataBody, '\0', COMM_MAX_SIZE);
//             // Split body data

//             // TODO: Memcopy
//             memcpy(PosDataSize, DataReceivedPos, POS_DATA_CHECK_SIZE);
//             memcpy(PosDataHeader, DataReceivedPos + POS_DATA_CHECK_SIZE + 1, POS_DATA_HEADER_SIZE);
//             memcpy(PosDataBody, DataReceivedPos + POS_DATA_CHECK_SIZE  + POS_DATA_HEADER_SIZE + 2, sizeof(DataReceivedPos) - POS_DATA_CHECK_SIZE - POS_DATA_HEADER_SIZE);
//             // Remove control character
//             PosDataBody[strlen(PosDataBody) - 1] = '\0';

//             // Size to int
//             int PosContentSize = atoi(PosDataSize);

// #if DEBUG_MODE
//     SPLOG_DEBUG("Size %s", PosContentSize);
//     SPLOG_DEBUG("Header: %s", PosDataHeader);
//     SPLOG_DEBUG("Body: %s", PosDataBody);
// #endif

//             // Check content body length
//             if (SignatureMain_checkMessageSize(PosDataBody, PosContentSize)) {
// #if WARNING_MODE
//     SPLOG_DEBUG("Error Size: Defined %d bytes; Received %d bytes", atoi(PosDataSize), strlen(PosDataBody));
// #endif
//                 // Size error = 2
//                 PosDataEmptyHeader[POS_DATA_HEADER_SIZE-1] = '2';
//                 SignatureMain_commSendSocket(idSocketAccepted, PosDataEmptyHeader);

//                 // Ommit this proccess
//                 SignatureMain_closeConnection(idSocketAccepted);

//                 continue;
//             }

//             // Check content type
//             // Los valores aceptados en el body son hexadecimales sin espacios ni nuevas lineas
//             res = 0;
//             if(signatureReceive = SignatureMain_checkMessageContent(PosDataBody, PosContentSize, &res)) {
//                 switch (signatureReceive) {
//                 case 4:
// #if WARNING_MODE
//     SPLOG_DEBUG("Error Char: Invalid character \"%c\"", (char)res);
// #endif
//                     // Char error = 4
//                     PosDataEmptyHeader[POS_DATA_HEADER_SIZE-1] = '4';
//                     break;

//                 case 5:
// #if WARNING_MODE
//     SPLOG_DEBUG("Error Odd: Body with odd size number (%d bytes)", res);
// #endif
//                     // Odd error = 5
//                     PosDataEmptyHeader[POS_DATA_HEADER_SIZE-1] = '5';
//                     break;
//                 }

//                 SignatureMain_commSendSocket(idSocketAccepted, PosDataEmptyHeader);

//                 // Ommit this proccess
//                 SignatureMain_closeConnection(idSocketAccepted);

//                 continue;
//             }

//             // Save file
//             if(SignatureMain_writeFile(INPUT_PATH, PosDataHeader, PosDataBody)) {
//                 SPLOG_ERROR("Error writing on file");
//                 SignatureMain_closeConnection(idSocketAccepted);
//                 SPLOG_CLOSE();
// 		        return BASE_PROCESSMANAGER_ERROR_CANNOT_CONTINUE;
//             }

//             // Response
//             memcpy(DataRespondPos, PosDataHeader, POS_DATA_HEADER_SIZE);

//             if(!FORCE_DISCONNECTION) {
//                 iRet = SignatureMain_commSendSocket(idSocketAccepted, PosDataHeader);

//                 if(iRet == COMM_ERROR_SOCKET_SEND) {
//                     SPLOG_ERROR("Error sending answer to ID: %s with PORT: %s", idSignatureSocket, portSocket);
//                     SignatureMain_closeConnection(idSocketAccepted);
//                     SPLOG_CLOSE();
//                     return BASE_PROCESSMANAGER_ERROR_SERVER_SOCKET;
//                 }
//             }
            
//             SPLOG_NOTICE("New signature: %s", PosDataHeader);

            
//             // Close connection (May don't need it)
//             SignatureMain_closeConnection(idSocketAccepted);
//         }

//     } while(1); //!base_process_isStopFlag());
// }
