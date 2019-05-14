CC=gcc
CFLAGS=-Wall -std=c99 -g
all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/json/json.c -c -o json.o
	$(CC) $(CFLAGS) src/tests/main.c *.o -o bin/json
	rm *.o
