CC=gcc

CFLAGS += -O2 -g -Wall -Wsign-compare -Wfloat-equal -Wformat-security
LDFLAGS += -lalleg

all:
	gcc $(CFLAGS) -c -o game.o game.c
	gcc -o game game.o $(LDFLAGS)

clean:
	-rm -f game


.PHONY: all install clean
