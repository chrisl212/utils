CC=gcc
CFLAGS=-Wall -std=c99 -g -lGL -lGLEW -lglfw -lm -I../ -Isrc/
all:
	mkdir -p bin
	$(CC) $(CFLAGS) ../mat/src/mat/*.c src/tests/*.c src/gl/*.c -o bin/gl

