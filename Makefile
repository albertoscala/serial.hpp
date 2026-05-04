CC     = gcc
CFLAGS = -Wall -Wextra -I./include

SRC     = src/serial.c
EXAMPLE = examples/example.c
TARGET  = bin/example

all: example

example: $(SRC) $(EXAMPLE)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) $(EXAMPLE) -o $(TARGET)

clean:
	rm -rf bin/

.PHONY: all example clean