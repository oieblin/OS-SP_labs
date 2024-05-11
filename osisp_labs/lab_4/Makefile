CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread -lrt

SRCS = main.c ring.c list.c
OBJS = $(SRCS:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean

