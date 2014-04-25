CC=colorgcc

CFLAGS += -O2 -g -Wall -Wsign-compare -Wfloat-equal -Wformat-security -std=gnu99
LDFLAGS += -lalleg -lm -lrt

all:
	$(CC) $(CFLAGS) -c -o game.o game.c
	$(CC) -o game game.o $(LDFLAGS)

clean:
	-rm -f game


.PHONY: all install clean
