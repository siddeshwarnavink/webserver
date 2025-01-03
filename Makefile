CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Wno-unused-parameter -g -ggdb -DDEBUG $(shell mysql_config --cflags)
LDFLAGS = $(shell mysql_config --libs)

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = bin/main

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) src/*.o

.PHONY: all debug clean
