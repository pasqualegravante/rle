#ifndef RLE_H
#define RLE_H
#include <stdio.h>

#define RLE_DEFAULT_BUFFER_SIZE 8192
#define RLE_MIN_RUN_LENGTH 3      // minimum length for an encoding run
#define RLE_MAX_RUN_LENGTH 255    // maximum length for an encoding run
#define RLE_CONTROL_BYTE 0x00     // control byte
#define RLE_SIGNATURE "RLE1"      // format signature

typedef struct {
    unsigned long buffer_size;
    unsigned long total_bytes_processed;
    unsigned long total_bytes_output;  
    unsigned char* buffer;
    FILE* input;
    FILE* output;
} RLEContext ;

#endif
