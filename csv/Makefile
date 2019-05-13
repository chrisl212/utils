CC=gcc
CFLAGS=-Wall -std=c99
all:
	$(CC) $(CFLAGS) src/csv/csv.c -c -o csv.o
	$(CC) $(CFLAGS) src/tests/main.c *.o -o bin/csv
	rm *.o
