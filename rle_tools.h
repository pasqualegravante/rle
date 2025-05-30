#ifndef RLE_TOOLS_H
#define RLE_TOOLS_H
#include "rle.h"
#include <stdlib.h>
#include <stdio.h>

enum RLEResult{
    RLE_OK = 0,            // success

    /* allocation failures */
    RLE_ENOMEM_CTX,        // malloc(sizeof(RLEContext)) failed
    RLE_ENOMEM_BUFFER,     // malloc(buffer_size) failed

    /* file‐open failures */
    RLE_EOPEN_INPUT,       // fopen(input_path,  "rb") failed
    RLE_EOPEN_OUTPUT,      // fopen(output_path, "wb") failed

    /* future I/O errors */
    RLE_EREAD,             // read/write error on input
    RLE_EWRITE,            // read/write error on output

    RLE_ECLOSE_INPUT,
    RLE_ECLOSE_OUTPUT,
    
    RLE_BAD_BUFFER_SIZE,

};

#ifdef RLE_TOOLS_INTERNAL
enum RLEResult rle_init(RLEContext** ctx, const char *input_path, const char *output_path, size_t buffer_size);
enum RLEResult rle_cleanup(RLEContext** context, _Bool deallocate_file_names);
#endif

void print_status(_Bool compression, const RLEContext* const context);

enum RLEResult rle_execute_init(RLEContext** ctx, const char *input_path, const char *output_path, size_t buffer_size);

enum RLEResult rle_execute_cleanup(RLEContext** context);

#endif