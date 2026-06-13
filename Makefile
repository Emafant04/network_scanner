CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC = src/port_scanner.c src/netutils.c
TARGET = bin/port_scanner

TEST_SRC = tests/test_netutils.c src/netutils.c
TEST_TARGET = bin/test_netutils

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all test clean
