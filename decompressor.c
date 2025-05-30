#include "decompressor.h"
#define true 1
#define false 0

enum RLEDecompressResult rle_decompress(RLEContext* const ctx){
    //reading file header RLE1<Original Size>
    //parsing RLE1 signature
    const int word_len=4;
    unsigned char word[word_len]={};
    
    fread(word, sizeof(*word), word_len, ctx->input);
    if(strncmp(word, RLE_SIGNATURE, word_len)!=0){//maybe later will re-implement strncmp, avoiding importing whole library
        //file not in RLE1 format
        printf("File format %s doesn't match %s\n", word, RLE_SIGNATURE);//DEBUG
        return RLE_DECOMPRESS_MISSING_SIGNATURE;
    }
    ctx->total_bytes_processed+=word_len;

    //parsing original file size
    uint32_t theoretical_decompressed_file_size;
    fread(&theoretical_decompressed_file_size, sizeof(theoretical_decompressed_file_size), 1, ctx->input);
    ctx->total_bytes_processed+=word_len;

    //NOTE: ctx->total_bytes_processed=8 at the start of the cycle
    //parsing remaining file
    const uint32_t fout_start=ftell(ctx->output);
    uint32_t num_read_bytes=0;
    uint8_t repeat=0;
    unsigned char c='\0';
    _Bool decode_next=false;

    while ((num_read_bytes = fread(ctx->buffer, sizeof(*ctx->buffer), ctx->buffer_size, ctx->input)) > 0){
        for(int i=0; i<num_read_bytes; i++){
            c=ctx->buffer[i];
            if(repeat!=0){
                for(int i=0; i<repeat; i++){
                    fputc(c, ctx->output);
                }
                repeat=0;
                decode_next=false;
            }
            else if(decode_next==true){
                repeat = (uint8_t)c;
            }
            else if(c==RLE_CONTROL_BYTE){
                decode_next=true;
            }
            else{
                fputc(c, ctx->output);
            }
        }
        ctx->total_bytes_processed+=num_read_bytes;
    }

    ctx->total_bytes_output=ftell(ctx->output)-fout_start;

    printf("\n%u/%u bytes found\n", ctx->total_bytes_output, theoretical_decompressed_file_size);

    if(ferror(ctx->input)){
        return RLE_DECOMPRESS_IO_INPUT;
    }
    if(ferror(ctx->output)){
        return RLE_DECOMPRESS_IO_OUTPUT;
    }

    if(ctx->total_bytes_output!=theoretical_decompressed_file_size){
        return RLE_DECOMPRESS_DATA_MAY_BE_LOST;
    }

    return RLE_DECOMPRESS_OK;
}

enum RLEDecompressResult rle_execute_decompression(RLEContext* const ctx){
    puts("-----DECOMPRESSION EXECUTION-----");
    printf("Decompressing using %u bytes buffer...\n", ctx->buffer_size);
    enum RLEDecompressResult res = rle_decompress(ctx);

    if(res==RLE_DECOMPRESS_OK){
        puts("Decompression went ok.");
        return res;
    }

    printf("Error[Decompression] with id %d: ", res);
    switch (res){
    case RLE_DECOMPRESS_IO_INPUT:
        puts("error at reading input file");
        break;
    
    case RLE_DECOMPRESS_IO_OUTPUT:
        puts("error at writing output file");
        break;
    
    case RLE_DECOMPRESS_MISSING_SIGNATURE:
        puts("error at finding RLE1 signature in input file");
        break;

    case RLE_DECOMPRESS_DATA_MAY_BE_LOST:
        puts("decompression executed, but some data might be lost");
        break;
    
    default:
        puts("unexpected error");
        break;
    }

    return res;
}