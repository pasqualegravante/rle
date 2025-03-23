#include <stdio.h>
#include <stdlib.h>
#include "compressor.h"

//suitable cheap boolean handling
#define true 1
#define false 0

int rle_compressor_init(RLEContext *ctx, const char *input_path, const char *output_path, size_t buffer_size);
int rle_compress(RLEContext *ctx);
void rle_compressor_cleanup(RLEContext *ctx);

int rle_compressor_init(RLEContext *ctx, const char *input_path, const char *output_path, size_t buffer_size){
    ctx->input=fopen(input_path, "rb");
    ctx->output=fopen(output_path, "wb");
    ctx->buffer_size=buffer_size;
    ctx->buffer=(unsigned char*)malloc(buffer_size);
    ctx->total_bytes_output=0;
    ctx->total_bytes_processed=0;

    return 20*(ferror(ctx->input) || ferror(ctx->output));//error code: 20, otherwise 0
}

int rle_compress(RLEContext* ctx){
    //since MAX_RUN_LENGTH is 255, one byte will perfectly fit for the purpose
    __uint8_t count=0;
    //flag used to regulate writing operation on file
    _Bool write = false;
    //adder to keep track of bytes_read
    size_t bytes_read;

    const int start_output = ftell(ctx->output);

    //calculating the input file dimension
    const int start_input = ftell(ctx->input);
    fseek(ctx->input, 0L, SEEK_END);
    const int end_input = ftell(ctx->input);
    rewind(ctx->input);
    const int flength_input=end_input-start_input;

    //writing output file header: RLE1<ORIGINAL_BYTE_DIMENSION>
    fwrite(RLE_SIGNATURE, 4, 1, ctx->output);
    fwrite(&flength_input, sizeof(flength_input), 1, ctx->output);
    
    while(!feof(ctx->input) && !ferror(ctx->input) && !ferror(ctx->output)){
        //read <unsigned char> sized byte for <ctx->buffer_size> times
        bytes_read = fread(ctx->buffer, sizeof(unsigned char), ctx->buffer_size, ctx->input);

        //cycle on the input buffer, it also cycles on spare 0's but doesn't write them
        for(int i=1; i<bytes_read; i++){

            //increase count whenever bytes are equals in pairs
            if(ctx->buffer[i]==ctx->buffer[i-1]) count++;

            //if current and previous bytes are different, then write
            if(ctx->buffer[i]!=ctx->buffer[i-1] || count==RLE_MAX_RUN_LENGTH){
                //in case of different consecutive bytes, remember to count this last byte of the current run
                count+=(ctx->buffer[i]!=ctx->buffer[i-1]);
                write=true;
            }

            if(write){
                //for a small count, write bytes normally
                if(count<RLE_MIN_RUN_LENGTH){
                    for(int j=0; j<count; j++)
                        fputc(ctx->buffer[i-1], ctx->output);
                }
                //encode longer strings of bytes
                else{
                    fputc(RLE_CONTROL_BYTE, ctx->output);
                    fputc(count, ctx->output);
                    fputc(ctx->buffer[i-1], ctx->output);
                }
                //update processed data bytes counter
                ctx->total_bytes_processed+=count;
                //count=1 since a new run begins immediately
                count=0;
                write=false;
            }
        }
        //ctx->total_bytes_processed+=bytes_read;
    }

    //DEBUG printf("\nbytes_read: %ld\n", bytes_read);

    if(ctx->total_bytes_processed!=bytes_read){//last write operation missed
        count=bytes_read-ctx->total_bytes_processed;//number of missed bytes

        if(count<RLE_MIN_RUN_LENGTH){
            for(int j=0; j<count; j++)
                fputc(ctx->buffer[bytes_read-1], ctx->output);
                //bytes_read-1 can be used as index for the last byte in buffer
        }

        else{
            fputc(RLE_CONTROL_BYTE, ctx->output);
            fputc(count, ctx->output);
            fputc(ctx->buffer[bytes_read-1], ctx->output);
        }
        //update processed data bytes counter
        ctx->total_bytes_processed+=count;
    }
    
    ctx->total_bytes_output=(ftell(ctx->output)-start_output);

    return 21*(ferror(ctx->input) || ferror(ctx->output)); //error code: 21, otherwise 0
}

void rle_compressor_cleanup(RLEContext *ctx){
    fclose(ctx->input);
    fclose(ctx->output);
    free(ctx->buffer);

    ctx->buffer=NULL;
    ctx->buffer_size=RLE_DEFAULT_BUFFER_SIZE;
    ctx->input=NULL;
    ctx->output=NULL;
    ctx->total_bytes_output=0;
    ctx->total_bytes_processed=0;
}