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
    RLEContext* context = (RLEContext*)malloc(sizeof(RLEContext));

    option = argv[1];
    input  = argv[2];
    output = argv[3];
    buffer_size = (argc==5 ? atoi(argv[4]) : RLE_DEFAULT_BUFFER_SIZE);

    //check if the option is -c or -d
    if(!(strcmp(option, "-c")==0 || strcmp(option, "-d")==0)){
        printf("Invalid option passed \'%s\': Expected -c or -d\n", argv[1]);
        rle_execute_cleanup(&context);
        return -2;
    }

    const enum RLEResult init_code = rle_execute_init(&context, input, output, buffer_size);
    if(init_code!=RLE_OK){
        rle_execute_cleanup(&context);
        return init_code;
    }
    /*USELESS BLOCK
    //check for wrong buffer_size(<=0)
    if(buffer_size<=0){
        printf("Invalid buffer_size %d, buffer size can't be less or equal than 0\n", buffer_size);
        rle_deallocate_resources(context);
        return 12;
    }
    if(buffer_size>RLE_DEFAULT_BUFFER_SIZE){
        printf("Invalid buffer_size %d, exceeding the default value: %d\n", buffer_size, RLE_DEFAULT_BUFFER_SIZE);
        rle_deallocate_resources(context);
        return 12;
    }*/

    puts("_RLE_Compressor_and_Decompressor_by_Pasquale_Gravante_");
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
    printf("Exit code: %d\n", status);
    return status;
}