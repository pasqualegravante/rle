#include "rle_tools.h"

int rle_init(RLEContext *ctx, const char *input_path, const char *output_path, size_t buffer_size){
    ctx->input=fopen(input_path, "rb");
    ctx->output=fopen(output_path, "wb");
    ctx->buffer_size=buffer_size;
    ctx->buffer=(unsigned char*)malloc(buffer_size);
    ctx->total_bytes_output=0;
    ctx->total_bytes_processed=0;

    return 20*(ferror(ctx->input) || ferror(ctx->output));//error code: 20, otherwise 0
}

void rle_cleanup(RLEContext *ctx){
    fclose(ctx->output);
    fclose(ctx->input);
    free(ctx->buffer);

    ctx->buffer=NULL;
    ctx->buffer_size=RLE_DEFAULT_BUFFER_SIZE;
    ctx->input=NULL;
    ctx->output=NULL;
    ctx->total_bytes_output=0;
    ctx->total_bytes_processed=0;
}

float get_compression_rate(const RLEContext* const context){
    return (float)context->total_bytes_output/context->total_bytes_processed*100;
}

void rle_deallocate_resources(RLEContext* context){
    rle_cleanup(context);
    free(context);
}