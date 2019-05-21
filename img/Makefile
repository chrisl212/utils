CC=gcc
CFLAGS=-Wall -std=c99 -lz -g
all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/img/*.c src/png/*.c src/jpg/*.c src/gif/*.c src/tests/main.c -o bin/img
