#include <assert.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "../src/netutils.h"

void test_my_htons() {
    assert(my_htons(0x0000) == htons(0x0000));
    assert(my_htons(0xFFFF) == htons(0xFFFF));
    assert(my_htons(0xABCD) == htons(0xABCD));

    printf("my_htons: OK\n");
}

void test_my_htonl() {
    assert(my_htonl(0x00000000) == htonl(0x00000000));
    assert(my_htonl(0xFFFFFFFF) == htonl(0xFFFFFFFF));
    assert(my_htonl(0x12345678) == htonl(0x12345678));

    printf("my_htonl: OK\n");
}

int main() {
    test_my_htons();
    test_my_htonl();

    printf("All tests passed.\n");
    return 0;
}
