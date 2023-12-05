#include <stdio.h>
#include <string.h>

#include "iso8583.h"
#include "dl_iso8583_common.h"
#include "dl_iso8583.h"

char* MTI_LIST[10] = {"1804", "1604", "1521", "1520", "1421", "1420", "1221", "1220", "1200", "1100"};


int check_mti(char* mti, char** mti_list, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (strcmp(mti, mti_list[i]) == 0) {
            return i;  // Se encontró el MTI en la posición i
        }
    }
    return -1;  // No se encontró el MTI en la lista
}

int bytesToInt(unsigned char* bytes, int size) {
    int result = 0;

    for (int i = 0; i < size; i++) {
        result = (result << 8) | bytes[i];
    }

    return result;
}

void intTo2Bytes(int value, unsigned char* bytes, int size) {
    for (int i = 0; i < size; i++) {
        bytes[i] = (unsigned char)((value >> (8 * (size - i - 1))) & 0xFF);
    }
}

void intTo2BytesReverse(int value, unsigned char* bytes, int size) {
    for (int i = 0; i < size; i++) {
        bytes[i] = (unsigned char)((value >> (8 * i)) & 0xFF);
    }
}

void bytesToHexString(unsigned char* bytes, int size, char* hexString) {
    for (int i = 0; i < size; i++) {
        sprintf(hexString + 2 * i, "%02X", bytes[i]);
    }
}

void bytesToHexStringBeauty(unsigned char* bytes, int size, char* hexString) {
    char* ptr = hexString;
    memset(hexString, '\0', sizeof(hexString));

    for (int i = 0; i < size; i++) {
        // Convert byte to hex and append to hexString
        // Add 0x00 and space after every byte
        // Add \n after every 16 bytes
        if (i % 16 == 0) {
            sprintf(ptr, "\n%02X ", bytes[i]);
            // 1 byte (\n) + 2 bytes (hex) + 1 byte (space) = 4 bytes
            ptr += 4;
        } else {
            sprintf(ptr, "%02X ", bytes[i]);
            // 2 bytes (hex) + 1 byte (space) = 3 bytes
            ptr += 3;
        }
    }
}

void hexStringToBytes(char* hexString, unsigned char* bytes) {
    for (int i = 0; i < strlen(hexString); i += 2) {
        sscanf(hexString + i, "%02hhX", &bytes[i / 2]);
    }
}

void fillWithMTI(char* mti, DL_ISO8583_MSG *bytes) {
    int nmti = atoi(mti);
    switch (nmti) {
        /* Autorization: Sale and refund */
        case 1100:
        /* Payment: Sale and refund*/
        case 1200:
            DL_ISO8583_MSG_SetField_Str(0, "1210", bytes);
            DL_ISO8583_MSG_SetField_Str(38, "123456", bytes);

            if (nmti == 1200) {
                DL_ISO8583_MSG_SetField_Str(39, "000", bytes);
                DL_ISO8583_MSG_SetField_Str(48, "01837************1234", bytes);
            } else {
                DL_ISO8583_MSG_SetField_Str(39, "085", bytes);
                DL_ISO8583_MSG_SetField_Str(48, "01952CS000111222333444", bytes);
            }

            // Remove 2 3 4 14 22 24 28 29 35 37 53 - Lenght 11
            int nullField1200[] = {2, 3, 4, 14, 22, 24, 28, 29, 35, 37, 53};

            for (size_t i = 0; i < sizeof(nullField1200) / sizeof(int); i++) {
                memset(&(*bytes).field[nullField1200[i]], '\0', sizeof(DL_ISO8583_MSG_FIELD));
            }
            break;
        
        /* Offline: Sale and refund */
        case 1220:
        /* Offline retry: Sale and refund */
        case 1221:
            DL_ISO8583_MSG_SetField_Str(0, "1230", bytes);
            DL_ISO8583_MSG_SetField_Str(39, "900", bytes);

            // Remove 22 24 25 35 37 38 53 55 - Lenght 8
            int nullField1220[] = {22, 24, 25, 35, 37, 38, 53, 55};

            for (size_t i = 0; i < sizeof(nullField1220) / sizeof(int); i++) {
                memset(&(*bytes).field[nullField1220[i]], '\0', sizeof(DL_ISO8583_MSG_FIELD));
            }
            break;

        /* Cancellation: Sale and refund */
        case 1420:
        /* Cancellation retry: Sale and refund */
        case 1421: 
        /* Close and open session */
        case 1520:
        /* Close and open session (Retry) */
        case 1521:
        /* Communication */
        case 1604:
        /* Echo message */
        case 1804:
            if (nmti == 1420 || nmti == 1421) {
                DL_ISO8583_MSG_SetField_Str(0, "1430", bytes);
                DL_ISO8583_MSG_SetField_Str(39, "400", bytes);
            } else
            if (nmti == 1520 || nmti == 1521) {
                DL_ISO8583_MSG_SetField_Str(0, "1530", bytes);
                DL_ISO8583_MSG_SetField_Str(39, "503", bytes);
            } else
            if (nmti == 1604) {
                DL_ISO8583_MSG_SetField_Str(0, "1614", bytes);
                DL_ISO8583_MSG_SetField_Str(39, "600", bytes);
            } 
            else {
                DL_ISO8583_MSG_SetField_Str(0, "1814", bytes);
                DL_ISO8583_MSG_SetField_Str(39, "800", bytes);
            }

            // Remove 24 48
            memset(&(*bytes).field[24], '\0', sizeof(DL_ISO8583_MSG_FIELD));
            memset(&(*bytes).field[48], '\0', sizeof(DL_ISO8583_MSG_FIELD));
            break;
            
        
        default:
            break;
    }
}
