CC=gcc
CFLAGS=-Wall -std=c99 -g
all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/lzw/*.c src/tests/main.c *.o -o bin/lzw
	rm *.o
