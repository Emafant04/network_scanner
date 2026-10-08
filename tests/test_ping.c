#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../src/ping.h"   /* deve dichiarare icmp_checksum() */

/* Scrive il checksum in memoria così com'è, per confrontare i BYTE */
static void checksum_bytes(const void *data, size_t len, uint8_t out[2])
{
    uint16_t c = icmp_checksum(data, len);
    memcpy(out, &c, 2);
}

static void test_checksum_rfc1071(void)
{
    const uint8_t buf[] = {0x00,0x01,0xf2,0x03,0xf4,0xf5,0xf6,0xf7};
    uint8_t out[2];
    checksum_bytes(buf, sizeof buf, out);
    assert(out[0] == 0x22 && out[1] == 0x0d);
}

static void test_checksum_odd_length(void)
{
    const uint8_t buf[] = {0x00,0x01,0xf2,0x03,0xf4,0xf5,0xf6};
    uint8_t out[2];
    checksum_bytes(buf, sizeof buf, out);
    assert(out[0] == 0x23 && out[1] == 0x04);
}

static void test_checksum_echo_request(void)
{
    const uint8_t buf[] = {0x08,0x00,0x00,0x00,0x00,0x01,0x00,0x01};
    uint8_t out[2];
    checksum_bytes(buf, sizeof buf, out);
    assert(out[0] == 0xf7 && out[1] == 0xfd);
}

static void test_checksum_verify_property(void)
{
    uint8_t pkt[] = {0x08,0x00,0x00,0x00,0x12,0x34,0x00,0x07,'p','i','n','g'};
    uint8_t c[2];
    checksum_bytes(pkt, sizeof pkt, c);
    pkt[2] = c[0];
    pkt[3] = c[1];
    assert(icmp_checksum(pkt, sizeof pkt) == 0);
}

static void test_checksum_edge_cases(void)
{
    const uint8_t zeros[8] = {0};
    assert(icmp_checksum(zeros, sizeof zeros) == 0xFFFF);
    assert(icmp_checksum(zeros, 0) == 0xFFFF);
}

int main(void)
{
    test_checksum_rfc1071();
    test_checksum_odd_length();
    test_checksum_echo_request();
    test_checksum_verify_property();
    test_checksum_edge_cases();
    printf("test_ping: tutti i test passati\n");
    return 0;
}
