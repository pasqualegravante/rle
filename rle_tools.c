#include "rle_tools.h"

enum RLEResult rle_init(RLEContext** ctx, const char *input_path, const char *output_path, size_t buffer_size){
    *ctx = (RLEContext*)malloc(sizeof(RLEContext));

    if(*ctx==NULL){
        return RLE_ENOMEM_CTX;
    }

    (*ctx)->input=fopen(input_path, "rb");
    if(ferror((*ctx)->input)!=0){
        return RLE_EOPEN_INPUT;
    }

    (*ctx)->output=fopen(output_path, "wb");
    if(ferror((*ctx)->output)!=0){
        return RLE_EOPEN_OUTPUT;
    }

    (*ctx)->buffer_size=buffer_size;
    if((*ctx)->buffer_size<=0 || (*ctx)->buffer_size>RLE_DEFAULT_BUFFER_SIZE){
        return RLE_BAD_BUFFER_SIZE;
    }

    (*ctx)->buffer=(unsigned char*)malloc(buffer_size);
    if((*ctx)->buffer==NULL){
        return RLE_ENOMEM_BUFFER;
    }

    (*ctx)->total_bytes_output=0;
    (*ctx)->total_bytes_processed=0;

    return RLE_OK;
}

enum RLEResult rle_cleanup(RLEContext** ctx, const _Bool deallocate_file_names){
    if(fclose((*ctx)->output)!=0){
        return RLE_ECLOSE_OUTPUT;
    }
    if(fclose((*ctx)->input)!=0){
        return RLE_ECLOSE_INPUT;
    }

    free((*ctx)->buffer); //no possible check, only error is it can be freed twice

    (*ctx)->buffer=NULL;
    (*ctx)->buffer_size=RLE_DEFAULT_BUFFER_SIZE;
    if(deallocate_file_names!=0){
        free((*ctx)->input);
        free((*ctx)->output);
        (*ctx)->input=NULL;
        (*ctx)->output=NULL;
    }
    (*ctx)->total_bytes_output=0;
    (*ctx)->total_bytes_processed=0;

    free(*ctx);
    //printf("OK dealloc of *ctx\n");
    //free(ctx);
    //printf("OK dealloc of ctx\n");
    return RLE_OK;
}

float get_rate(const RLEContext* const context){
    return (float)context->total_bytes_output/context->total_bytes_processed;
}

enum RLEResult rle_execute_init(RLEContext** ctx, const char *input_path, const char *output_path, size_t buffer_size){
    enum RLEResult res = rle_init(ctx, input_path, output_path, buffer_size);
    
    if(res==RLE_OK){
        return res;
    }

    printf("Error[RLE Init] with id %d: ", res);
    switch (res)
    {
    case RLE_ENOMEM_CTX:
        puts("insufficient heap memory to allocate RLEContext");
        break;
    
    case RLE_ENOMEM_BUFFER:
        puts("insufficient heap memory to allocate buffer of RLEContext");
        break;
    
    case RLE_EOPEN_INPUT:
        puts("couldn't open input file in `rb` mode (read-byte)");
        break;
    
    case RLE_EOPEN_OUTPUT:
        puts("couldn't open output file in `wb` mode (write-byte)");
        break;
    
    case RLE_BAD_BUFFER_SIZE:
        printf("buffer size must be in range [1, %d]", RLE_DEFAULT_BUFFER_SIZE);
        break;

    default:
        puts("unknown error");
        break;
    }
    puts("\n");

    return res;
}

enum RLEResult rle_execute_cleanup(RLEContext** context){
    enum RLEResult res = rle_cleanup(context, 1);

    if(res==RLE_OK){
        return res;
    }

    printf("Error[RLE CleanUp] with id %d: ", res);
    switch (res)
    {
    case RLE_ECLOSE_INPUT:
        puts("couldn't close input file");
        break;
    
    case RLE_ECLOSE_OUTPUT:
        puts("couldn't close output file");
        break;

    default:
        puts("unknown error");
        break;
    }
    puts("\n");
    return res;
}

void print_status(_Bool compression, const RLEContext* const context){
    puts("\n----- REPORT -----");
    printf("Bytes processed:\t%u\n", context->total_bytes_processed);
    printf("Bytes in output:\t%u\n", context->total_bytes_output);
    float operation_rate = (compression==0 ? 1-get_rate(context) : get_rate(context))*100;
    printf("%s rate:\t%0.3f%%\n", (compression==0 ? "Compression" : "Decompression"), operation_rate);
}