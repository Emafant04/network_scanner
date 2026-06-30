#include <assert.h>
#include <stdio.h>

#include "../src/scan.h"
#include "../src/netutils.h"

static void test_scan_port(void) {
    printf("--- scan_port manual tests ---\n");
    printf("open port     127.0.0.1:1234 : %d (expected %d)\n", scan_port("127.0.0.1", 1234), PORT_OPEN);
    printf("closed port   127.0.0.1:9999 : %d (expected %d)\n", scan_port("127.0.0.1", 9999), PORT_CLOSED);
    printf("invalid ip    not_an_ip:80   : %d (expected %d)\n", scan_port("not_an_ip", 80),    PORT_ERROR);
    printf("--- end scan_port tests ---\n");
}

static void test_parse_range(void) {
    int start, end;

    assert(parse_range("80", &start, &end) == 0);
    assert(start == 80 && end == 80);

    assert(parse_range("20-80", &start, &end) == 0);
    assert(start == 20 && end == 80);

    assert(parse_range("1024-", &start, &end) == 0);

    assert(parse_range(NULL, &start, &end) == -1);

    assert(parse_range("abc", &start, &end) == -1);

    assert(parse_range("80-20", &start, &end) == -1);

    printf("parse_range: ok\n");
}

int main(void) {
    test_parse_range();
    test_scan_port();

    printf("All tests passed.\n");
    return 0;
}
