/*
    WARNING: this file is in early development, unstable due to:
    - memory unsafe
    - missing some compression tests
    - missing all decompression tests
*/
#define true 1
#define false 0
#define COMPRESSOR_INTERNAL
#define DECOMPRESSOR_INTERNAL
#define RLE_TOOLS_INTERNAL

#include "rle.h"
#include "rle_tools.h"
#include "compressor.h"
#include "decompressor.h"

#include <string.h>
#include <assert.h>

//FILE SPECIFICATIONS
const int NO_TEST_FILE = 5;
const int NO_TEST_BUFFER = 6;
const char const* FILE_BASE_PATH = "./tests/";

//VARIANTS
RLEContext* rle;
int buffer_dimensions[] = {RLE_DEFAULT_BUFFER_SIZE, 67, 1, 1, 128, RLE_DEFAULT_BUFFER_SIZE};

void test_compression_file_n(unsigned int n);
int main(){
    printf("|===== TESTING FILES in %s directory =====|\n", FILE_BASE_PATH);
    puts("\tCompression testing");
    for(int i=1; i<=NO_TEST_FILE; i++){
        test_compression_file_n(i);
        printf("* %d/%d file tested\n", i, NO_TEST_FILE);
    }
}

char* itoa(int number, unsigned int* num_of_digits){
    unsigned int digits = 1;
    int num = number;
    while(num/10!=0){
        number/=10;
        digits++;
    }

    num=number;
    char* converted = malloc(sizeof(char) * (digits+1));
    for(int i=digits-1; i>=0; i--){
        converted[i]=num%10 + '0';
        num/=10;
    }

    converted[digits]='\0';
    *num_of_digits = digits;
    return converted;
}

char* build_file_name(const char* base_name, const int file_number, const char* file_format){
    unsigned int num_of_digits = 0;
    char* fn_num = itoa(file_number, &num_of_digits);
    //printf("num of digits: %d\n", num_of_digits);
    const int file_name_length = sizeof(FILE_BASE_PATH) + sizeof(base_name) + num_of_digits + sizeof(file_format) -3*sizeof((char)'\0') + sizeof((char)'\0');
    //need to perform cast at `sizeof((char)'\0')` to prevent compiler assuming it as an integer
    //need to subtract `sizeof((char)'\0')` for 3 times since each `char* variable` carries `\0`
    
    char* file_name = malloc(sizeof(char) * file_name_length);
    strcat(file_name, FILE_BASE_PATH);
    strcat(file_name, base_name);
    strcat(file_name, fn_num);
    strcat(file_name, file_format);
    file_name[file_name_length]='\0';

    free(fn_num);
    return file_name;

}

void test_compression_file_n(const unsigned int n){
    printf(">>Testing file no. %d\n", n);
    int tested_buffer = 0;
    //printf("here ok 2\n");
    char* input = build_file_name("test", n, ".txt");
    //printf("here ok 1\n");
    char* output = build_file_name("out", n, ".bin");
    //printf("here ok\n");
    printf("Input file:%s\n",input);
    printf("Output file:%s\n\n",output);

    for(int i=0; i<NO_TEST_BUFFER; i++){
        printf("Test no. (%d), buffer dimension: %d", i, buffer_dimensions[i]);
        assert(rle_init(&rle, input, output, buffer_dimensions[i])==RLE_OK);
        assert(rle_compress(rle)==RLE_COMPRESS_OK);
        //add checks on rle context and supposed files' outputs
        assert(rle_cleanup(&rle, false)==RLE_OK);
        tested_buffer++;
        printf("\tPASSED\n");
    }

    printf("Finished testing file: %d/%d\n", tested_buffer, NO_TEST_BUFFER);

    free(input);
    free(output);
}

/*
char* _strcat(char* str1, char* str2, int cat_start_idx){
    unsigned int str1_len = _strlen(str1);
    unsigned int str2_len = _strlen(str2);
    unsigned int str3_len = str1_len+str2_len-1;//removing one duplicate of `\0`

    char* str3 = malloc(sizeof(char) * str3_len);
    for(int i=0; i<str3_len; i++){
        str3[i] = (i<str1_len ? str1[i] : str2[i%(str1_len-1)]);
    }

    return str3;
}

unsigned int _strlen(char* str){
    unsigned int len = 0;
    for(;str[len]!='\0'; len++) len++;
    return len+1;//counting the ending char
}*/


