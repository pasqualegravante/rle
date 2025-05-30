#include "compressor.h"

//suitable cheap boolean handling
#define true 1
#define false 0

enum RLECompressResult rle_compress(RLEContext* const ctx){
    //since MAX_RUN_LENGTH is 255, one byte will perfectly fit for the purpose
    uint8_t count=0;
    //flag used to regulate writing operation on file
    _Bool write = false;
    //adder to keep track of bytes_read
    uint32_t bytes_read;

    const int start_output = ftell(ctx->output);

    //calculating the input file dimension
    const int start_input = ftell(ctx->input);
    fseek(ctx->input, 0L, SEEK_END);
    const int end_input = ftell(ctx->input);
    rewind(ctx->input);
    const uint32_t flength_input=end_input-start_input;

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

                count=0;
                write=false;
            }
        }
        //ctx->total_bytes_processed+=bytes_read;
    }

    //DEBUG printf("\nbytes_read: %ld\n", bytes_read);

    if(ctx->total_bytes_processed!=bytes_read){//last write operation missed
        count=bytes_read - ctx->total_bytes_processed;//number of missed bytes

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
    
    if(ferror(ctx->input)){
        return RLE_COMPRESS_IO_INPUT;
    }

    if(ferror(ctx->output)){
        return RLE_COMPRESS_IO_OUTPUT;
    }

    return RLE_COMPRESS_OK;
}

enum RLECompressResult rle_execute_compression(RLEContext* const ctx){
    puts("-----COMPRESSION EXECUTION-----");
    printf("Compressing using %u bytes buffer...\n", ctx->buffer_size);
    enum RLECompressResult res = rle_compress(ctx);
    
    if(res==RLE_COMPRESS_OK){
        puts("Compression went ok.");
        return res;
    }

    printf("Error[Compression] with id %d: ", res);
    switch (res){
    case RLE_COMPRESS_IO_INPUT:
        puts("error at reading input file");
        break;
    
    case RLE_COMPRESS_IO_OUTPUT:
        puts("error at writing output file");
        break;
    
    default:
        puts("unknown error");
        break;
    }

    return res;
}