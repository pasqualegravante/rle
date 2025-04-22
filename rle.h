#ifndef RLE_H
#define RLE_H
#include <stdio.h>
#include <stdint.h>

#define RLE_DEFAULT_BUFFER_SIZE 8192
#define RLE_MIN_RUN_LENGTH 3      // minimum length for an encoding run
#define RLE_MAX_RUN_LENGTH 255    // maximum length for an encoding run
#define RLE_CONTROL_BYTE 0x00     // control byte
#define RLE_SIGNATURE "RLE1"      // format signature

typedef struct {
    uint32_t buffer_size;
    uint32_t total_bytes_processed;
    uint32_t total_bytes_output;  
    unsigned char* buffer;
    FILE* input;
    FILE* output;
} RLEContext ;

#endif
