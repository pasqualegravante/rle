#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include "rle.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum RLEDecompressResult{
    RLE_DECOMPRESS_OK = 0,
    RLE_DECOMPRESS_MISSING_SIGNATURE,
    RLE_DECOMPRESS_DATA_MAY_BE_LOST,
    RLE_DECOMPRESS_IO_INPUT,
    RLE_DECOMPRESS_IO_OUTPUT,
};

#ifdef DECOMPRESSOR_INTERNAL
enum RLEDecompressResult rle_decompress(RLEContext* const ctx);
#endif

enum RLEDecompressResult rle_execute_decompression(RLEContext* const ctx);

#endif