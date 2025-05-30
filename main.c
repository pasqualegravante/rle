#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rle.h"
#include "rle_tools.h"
#include "compressor.h"
#include "decompressor.h"

int main(int argc, char* argv[]){ 
    //check expression rightness
    if(!(argc==4 || argc==5)){
        printf("Usage: %s (-c|-d) <input> <output> [buffer_size]\n", argv[0]);
        return -1;
    }

    const char* option;
    const char* input;
    const char* output;
    unsigned int buffer_size;
    RLEContext* context;

    option = argv[1];
    input  = argv[2];
    output = argv[3];
    buffer_size = (argc==5 ? atoi(argv[4]) : RLE_DEFAULT_BUFFER_SIZE);

    //check if the option is -c or -d
    if(strcmp(option, "-c") && strcmp(option, "-d")){
        printf("Invalid option passed \'%s\': Expected -c or -d\n", argv[1]);
        rle_execute_cleanup(&context);
        return -2;
    }

    const enum RLEResult init_code = rle_execute_init(&context, input, output, buffer_size);
    if(init_code!=RLE_OK){
        rle_execute_cleanup(&context);
        return init_code;
    }

    puts("_RLE_Compressor_and_Decompressor");
    printf("I'm working on file \'%s\' as input and \'%s\' as output...\n\n", input, output);
    int status=0;

    if(strcmp(option, "-c")==0){
        const enum RLECompressResult compress_res = rle_execute_compression(context);
        if(compress_res==RLE_COMPRESS_OK){
            print_status(0, context);
        }
        status=compress_res;
    }

    else{
        const enum RLEDecompressResult decompress_res = rle_execute_decompression(context);
        if(decompress_res==RLE_DECOMPRESS_OK){
            print_status(1, context);
        }
        status=decompress_res;
    }

    rle_execute_cleanup(&context);
    return status;
}