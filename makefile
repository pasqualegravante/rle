ifeq ($(OS),Windows_NT)
    EXT = .exe
else
    EXT = .out
endif

CC = gcc
CFLAGS = -O2
SRC = main.c rle_tools.c compressor.c decompressor.c
OUT = rle$(EXT)

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)