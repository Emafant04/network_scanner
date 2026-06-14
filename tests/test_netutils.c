#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <sys/socket.h>
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
void test_my_inet_pton() {
    uint32_t my_result;
    uint32_t ref_result;

    // caso corretto — confronto binario con inet_pton
    assert(my_inet_pton(AF_INET, "192.168.1.1", &my_result) == 1);
    inet_pton(AF_INET, "192.168.1.1", &ref_result);
    assert(my_result == ref_result);

    // af non riconosciuto
    assert(my_inet_pton(42, "192.168.1.1", &my_result) == -1);

    // lettere al posto di numeri
    assert(my_inet_pton(AF_INET, "192.abc.1.1", &my_result) == 0);

    // numero fuori range
    assert(my_inet_pton(AF_INET, "192.168.1.999", &my_result) == 0);

    // stringa senza punti
    assert(my_inet_pton(AF_INET, "192168011", &my_result) == 0);

    // puntatore src NULL
    assert(my_inet_pton(AF_INET, NULL, &my_result) == -1);

    printf("my_inet_pton: OK\n");
}

int main() {
    test_my_htons();
    test_my_htonl();
    test_my_inet_pton();

    printf("All tests passed.\n");
    return 0;
}
