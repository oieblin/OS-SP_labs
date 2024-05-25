CC = gcc
CFLAGS = -pthread
.PHONY: clean

all: prog reader generator
prog: main.c mainFunctions.h mainFunctions.c structures.h Makefile
	$(CC) $(CFLAGS) main.c mainFunctions.c -o prog
reader: binReader.c structures.h Makefile
	$(CC) $(CFLAGS) binReader.c -o reader
generator: generator.c structures.h Makefile
	$(CC) $(CFLAGS) generator.c -o generator
clean: 
	rm prog reader generator
