CC=colorgcc

C_FILES := $(wildcard *.c)
OBJ_FILES := $(notdir $(C_FILES:.c=.o))

CFLAGS += -O2 -g -Wall -Wsign-compare -Wfloat-equal -Wformat-security -std=gnu99
LDFLAGS += -lalleg -lm -lrt

game: $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f game


.PHONY: all install clean
