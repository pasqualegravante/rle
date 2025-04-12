#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "rle.h"
#include <stdio.h>
#include <stdlib.h>

int rle_compress(RLEContext* const ctx);

#endif