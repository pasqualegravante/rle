#ifndef RLE_TOOLS_H
#define RLE_TOOLS_H
#include "rle.h"
#include <stdlib.h>
#include <stdio.h>

float get_compression_rate(const RLEContext* const context);
int rle_init(RLEContext *ctx, const char *input_path, const char *output_path, size_t buffer_size);
void rle_deallocate_resources(RLEContext* context);

#endif