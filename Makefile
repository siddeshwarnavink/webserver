CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g -O2

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = bin/main

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(DEBUG_TARGET) src/*.o

.PHONY: all debug clean
