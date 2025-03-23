#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include "rle.h"

int rle_compressor_init(RLEContext *ctx, const char *input_path, const char *output_path, size_t buffer_size);
int rle_compress(RLEContext *ctx);
void rle_compressor_cleanup(RLEContext *ctx);

#endif