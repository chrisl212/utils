CC=gcc
CFLAGS=-Wall -std=c99 -lm -g
all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/mat/mat.c -c -o mat.o
	$(CC) $(CFLAGS) src/mat/vec.c -c -o vec.o
	$(CC) $(CFLAGS) src/tests/main.c *.o -o bin/mat
	rm *.o
