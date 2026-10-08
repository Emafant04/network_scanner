#include <stdint.h>
#include <sys/socket.h>
#include <stddef.h>
#include <string.h>
#include <netinet/in.h>

int ping_host(char *ip){
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    return 0;

}
uint16_t icmp_checksum(const void *data, size_t len){
    const uint8_t *p = (const uint8_t *)data;
    uint32_t sum = 0;

    /* 1. Somma a coppie di byte */
    while (len > 1) {
        uint16_t word;
        memcpy(&word, p, 2);     // vedi nota sotto
        sum += word;
        p   += 2;
        len -= 2;
    }

    /* 2. Byte rimasto (lunghezza dispari) */
    if (len == 1) {
        uint16_t last = 0;
        memcpy(&last, p, 1);     // in memoria diventa [byte, 0x00]
        sum += last;
    }

    /* 3. Ripiegamento dei riporti */
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    /* 4. Complemento a 1, troncato a 16 bit */
    return (uint16_t)~sum;
}
