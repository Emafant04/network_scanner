CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC = src/port_scanner.c
TARGET = bin/port_scanner

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
