#include <stdio.h>
#include "rle.h"

size_t strlen(const char *str) {
    const char *s;
    for (s = str; *s; ++s);
    return(s - str);
}

int main(int argc, char* argv[]){
    printf("size of FILE: %d\n", sizeof(FILE));
    printf("size of pointer: %d\n", sizeof(void*)); //8 bytes length for any pointer no matter the type pointed to, be careful, even if the pointer's size is 8, it doesn't mean that in the current machine that a memory cell is addressed by an 8 bytes address

    //check expression rightness
    if(argc!=3){
        printf("Usage: %s (-c|-d) <input>\n", argv[0]);
        return 1;
    }

    //check if the option is -c or -d
    if(argv[1][0]!='-' || strlen(argv[1])!=2 || !(argv[1][1]=='c' || argv[1][1]=='d')){
        printf("Invalid option passed \'%s\': Expected -c or -d\n", argv[1]);
        return 1;
    }

    //try to open the file in read mode
    FILE* input = fopen(argv[2], "r");
    if(input == NULL || ferror(input)){
        printf("Error opening file \'%s\' in read mode\n", argv[2]);
        return 2;
    }

    printf("I'm working on file \'%s\'...\n", argv[2]);

    if(argv[1][1]=='c'){
        printf("Compressing...\n");

    }

    if(argv[1][1]=='c'){
        printf("Decompressing...\n");

    }

    fclose(input);

    //printf("size of RLEContext: %d\n", sizeof(RLEContext));
    return 0;
}