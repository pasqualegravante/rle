#ifndef RLE_H
#define RLE_H

#define RLE_DEFAULT_BUFFER_SIZE 8192
#define RLE_MIN_RUN_LENGTH 3      // Minima lunghezza per comprimere una sequenza
#define RLE_MAX_RUN_LENGTH 255    // Massima lunghezza in una singola coppia RLE
#define RLE_CONTROL_BYTE 0x00     // Byte di controllo per modalità letterale
#define RLE_SIGNATURE "RLE1"      // Firma del formato
#include <stdio.h>

typedef struct {
    unsigned long buffer_size;
    unsigned long total_bytes_processed;
    unsigned long total_bytes_output;  
    unsigned char* buffer;
    FILE* input;
    FILE* output;
} RLEContext ;

#endif
