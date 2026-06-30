CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC = src/port_scanner.c src/netutils.c src/scan.c
TARGET = bin/port_scanner

TEST_NETUTILS_SRC = tests/test_netutils.c src/netutils.c
TEST_NETUTILS_TARGET = bin/test_netutils

TEST_SCAN_SRC = tests/test_scan.c src/scan.c src/netutils.c
TEST_SCAN_TARGET = bin/test_scan

all: $(TARGET)

$(TARGET): $(SRC) | bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TEST_NETUTILS_TARGET) $(TEST_SCAN_TARGET)
	./$(TEST_NETUTILS_TARGET)
	./$(TEST_SCAN_TARGET)

$(TEST_NETUTILS_TARGET): $(TEST_NETUTILS_SRC)
	$(CC) $(CFLAGS) $(TEST_NETUTILS_SRC) -o $(TEST_NETUTILS_TARGET)

$(TEST_SCAN_TARGET): $(TEST_SCAN_SRC)
	$(CC) $(CFLAGS) $(TEST_SCAN_SRC) -o $(TEST_SCAN_TARGET)

clean:
	rm -f $(TARGET) $(TEST_NETUTILS_TARGET) $(TEST_SCAN_TARGET)

bin:
	mkdir -p bin

.PHONY: all test clean
