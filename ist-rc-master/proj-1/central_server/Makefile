CC       = gcc
CFLAGS   = -g -std=c99 -Wall -pedantic

.PHONY: all clean zip

all: test_user test_bs

test_user: test_user.c
	$(CC) $(CFLAGS) -o $@ $+

test_bs: test_bs.c
	$(CC) $(CFLAGS) -o $@ $+

clean:
	rm -rf test_user test_bs
