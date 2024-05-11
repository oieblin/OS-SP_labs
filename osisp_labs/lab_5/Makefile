SRCS = main.c ring.c routines.c
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
