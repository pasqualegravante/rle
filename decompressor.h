#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include "rle.h"
#include <stdlib.h>
#include <stdio.h>

int rle_decompress(RLEContext* const ctx);

#endif