#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rle.h"
#include "compressor.h"

void deallocate_resources(RLEContext* context){
    rle_compressor_cleanup(context);
    free(context);
}

int main(int argc, char* argv[]){ 
    //check expression rightness
    if(!(argc==4 || argc==5)){
        printf("Usage: %s (-c|-d) <input> <output> [buffer_size]\n", argv[0]);
        return 10;
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
        deallocate_resources(context);
        return 11;
    }

    //check for wrong buffer_size(<=0)
    if(buffer_size<=0){
        printf("Invalid buffer_size %d, buffer size can't be less or equal than 0\n", buffer_size);
        deallocate_resources(context);
        return 12;
    }
    if(buffer_size>RLE_DEFAULT_BUFFER_SIZE){
        printf("Invalid buffer_size %d, exceeding the default value: %d\n", buffer_size, RLE_DEFAULT_BUFFER_SIZE);
        deallocate_resources(context);
        return 12;
    }

    printf("_RLE_Compressor_and_Decompressor_by_Pasquale_Gravante_\n");
    printf("I'm working on file \'%s\' as input and \'%s\' as output...\n", input, output);

    if(strcmp(option, "-c")==0){
        printf("Compressing using %d bytes buffer...\n", buffer_size);
        //get the result of the operation
        int compressor_init_code = rle_compressor_init(context, input, output, buffer_size);

        //operation of initiating the compressor went well
        if(compressor_init_code==0){
            int compressor_code = rle_compress(context);
            
            //operation of compressing the file went well
            if(compressor_code==0){
                printf("Successfully compressed.\n");
                printf("\n-----COMPRESSION REPORT-----\n");
                printf("Bytes processed:   %ld\n", context->total_bytes_processed);
                printf("Bytes in output:   %ld\n", context->total_bytes_output);
                printf("Compression rate:  %0.3f%%\n", ((float)context->total_bytes_output/context->total_bytes_processed)*100);
            }
            else{
                printf("Error in read/write operation during compression... try again.\n");
                deallocate_resources(context);
                return compressor_code;
            }
        }
        else{
            printf("Error in initializing the compressor... try again.\n");
            deallocate_resources(context);
            return compressor_init_code;
        }
    }

    else{
        printf("Decompressing...\n");
        return 3;//type error 3*
    }

    deallocate_resources(context);
    return 0;
}