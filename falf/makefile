CC = gcc
CFLAGS = -Wall -Wextra

all: banker

banker: banker.c
	$(CC) $(CFLAGS) -o banker banker.c

clean:
	rm -f banker result.txt

.PHONY: all clean
