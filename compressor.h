#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "rle.h"
#include <stdio.h>
#include <stdlib.h>

enum RLECompressResult{
    RLE_COMPRESS_OK = 0,
    RLE_COMPRESS_IO_OUTPUT,
    RLE_COMPRESS_IO_INPUT
};

#ifdef COMPRESSOR_INTERNAL
enum RLECompressResult rle_compress(RLEContext *ctx);
#endif

enum RLECompressResult rle_execute_compression(RLEContext* const ctx);

#endif