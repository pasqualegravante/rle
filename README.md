# RLE encoder/decoder
A custom Run-Length Encoding (RLE) encoder and decoder implemented in C.  
The project focuses on handling raw data streams, performing I/O operations and working directly with stack/heap memory.

## Table of Contents
- [Status](#status)
- [Key learning points](#key-learning-points)
- [Key features](#key-features)
- [How to Compile and Run](#how-to-compile-and-run)
- [Author](#author)
- [Badges](#badges)
- [License](#license)
  
## Status
This project is WIP. The core logic is implemented, now I'm trying to develop new features like:
- manual testing
- possibility of giving different buffer size ot use in I/O operations
- when no buffer size is explicitly provided via command line, calculate a smart size instead of using the default one
  
## Key learning points
- Manual memory management and pointer manipulation
- Understanding of stack vs heap behavior
- Use of enums for error handling
- Interface abstraction: wrapper functions hide internal implementation and reduce coupling
- Simulated pass-by-reference using `RLEContext**` to allow pointer reassignment

## Key features
Here there are some remarkable code snippets of the project.

### Cross-platform file format
The file format consists of 8 bytes:
- 4 bytes: "RLE1" signature
- 4 bytes: the original file byte-size as a `uint32_t`

This design ensures cross-platform compatibility because:
- "RLE1" is written in raw ASCII bytes, consistent across major most of OS
- `uint32_t` forces the file size to be stored as a 4-byte unsigned integer, no matter the architecture

*Note: the file size is at maximum (2^32-1)/2^30\~2^2\~4GB*

However the only issue that persists is endianness, in order to achieve a full platform independence, a conversion must be made when reading/writing the file.

rle.h
```C
#define RLE_SIGNATURE "RLE1"      // format signature

typedef struct {
    uint32_t buffer_size;
    uint32_t total_bytes_processed;
    uint32_t total_bytes_output;  
    unsigned char* buffer;
    FILE* input;
    FILE* output;
} RLEContext ;

#endif
```

### Abstractions for `rle_compress`,`rle_decompress`,`rle_init`,`rle_cleanup`
The abstractions made for these four functions can be equivalently shown, basically all of them aim at:
- reducing caller complexity: caller doesn't handle every callee's failure
- centralize function execution: group related logic together in the wrapper function, this translates also into a better error handling like the previous point 
compressor.c
```C
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
```

## How to Compile and Run
After cloning or downloading the repository, follow the steps below to compile and run the project.
### Compilation
It is possible to run the `make` command, otherwise you might follow the steps below...

On Linux and macOS:
```bash
gcc -O2 -o rle.out main.c rle_tools.c compressor.c decompressor.c
```
On Windows (with MinGW or WSL):
```bash
gcc -O2 -o rle.exe main.c rle_tools.c compressor.c decompressor.c
```

### Run
In order to run the program, the user must specify the following:
- **compression flag**: it can be either `-c` for compression or `-d` for decompression
- **input file**: a valid path to the input file
- **output file**: a valid path to the output file
- (optional) **buffer size**: the size of the buffer used for I/O operations, it must be in the interval [1, 8192]

*NOTEs:*
- negative rate can happen only during compression and it means that the file grew (try with test1.txt)
- input file size must be under 4GB due to signature limitation, see [why](#cross-platform-file-format)

**WARNING: using different sized-buffer might produce different outputs, this feature is working in progress. For reproducibility and debugging, it’s recommended to use the default buffer size unless explicitly testing different ones.**

#### Example#1
input
```bash
./rle.out -c ./tests/test1.txt ./tests/out1.bin
```
output
```
_RLE_Compressor_and_Decompressor_by_Pasquale_Gravante
I'm working on file './tests/test1.txt' as input and './tests/out1.bin' as output...

-----COMPRESSION EXECUTION-----
Compressing using 8192 bytes buffer...
Compression went ok.

----- REPORT -----
Bytes processed:        17
Bytes in output:        23
Compression rate:       -35.294%
```
#### Example#2
input
```bash
./rle.out -c ./tests/test3.txt ./tests/out3.bin 32
```
output
```
_RLE_Compressor_and_Decompressor_by_Pasquale_Gravante
I'm working on file './tests/test3.txt' as input and './tests/out3.bin' as output...

-----COMPRESSION EXECUTION-----
Compressing using 32 bytes buffer...
Compression went ok.

----- REPORT -----
Bytes processed:        260
Bytes in output:        23
Compression rate:       91.154%
```

#### Example#2bis
input
```bash
./rle.out -c ./tests/test3.txt ./tests/out3.bin
```
output
```
_RLE_Compressor_and_Decompressor_by_Pasquale_Gravante
I'm working on file './tests/test3.txt' as input and './tests/out3.bin' as output...

-----COMPRESSION EXECUTION-----
Compressing using 8192 bytes buffer...
Compression went ok.

----- REPORT -----
Bytes processed:        260
Bytes in output:        23
Compression rate:       91.154%
```

#### Example 2 and 2bis show the announced warning
In these two examples it seems that conversion happened in the same way, but in reality when the `hexdump` of their output files are checked, the result is two different compressions being made. 

With default size
  original
```
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
```
  compressed
```hex
0000000 4c52 3145 0104 0000 4c00 0a41 3700 0a42
0000010 4600 0a43 3800 0044                    
0000017
```

With size 32
original
```
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
```
  compressed
```hex
0000000 4c52 3145 0104 0000 4a00 0a41 3500 0a42
0000010 4400 0a43 3e00 0044                    
0000017
```

Let's try and see what's in the 'size 32' compressed by decompressing it with default buffer size in order to avoid propagation of chained errors:
```bash
./rle.out -d ./tests/out3.bin ./tests/test3.txt
```
  decompressed
```
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
```
Instantly is noticeable that for some reason D's are more than the original file but it has the same size, so the other letters must be less than what is supposed to be,

## Author
👋 Hi, I’m a first-year Computer Science student passionate about understanding how things work under the hood.  
This project reflects both what I’ve learned and what I’m still discovering.  

Feel free to check out the code or leave suggestions.

## Badges
![WIP](https://img.shields.io/badge/status-WIP-yellow)
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)

## License
MIT License

Copyright (c) 2025 Pasquale Gravante

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

