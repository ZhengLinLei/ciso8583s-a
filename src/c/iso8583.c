#include <stdio.h>
#include <string.h>

#include "iso8583.h"

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

void bytesToHexString(unsigned char* bytes, int size, char* hexString) {
    for (int i = 0; i < size; i++) {
        sprintf(hexString + 2 * i, "%02X", bytes[i]);
    }
}

void bytesToHexStringBeauty(unsigned char* bytes, int size, char* hexString) {
    char* ptr = hexString;
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

    hexString[3 * size - 1] = '\0'; // Null-terminate the string
}

void hexStringToBytes(char* hexString, unsigned char* bytes) {
    for (int i = 0; i < strlen(hexString); i += 2) {
        sscanf(hexString + i, "%02hhX", &bytes[i / 2]);
    }
}