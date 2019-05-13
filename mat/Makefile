CC=gcc
CFLAGS=-Wall -std=c99
all:
	$(CC) $(CFLAGS) src/mat/mat.c -c -o mat.o
	$(CC) $(CFLAGS) src/tests/main.c *.o -o bin/mat
	rm *.o
