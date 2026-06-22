#include <stdint.h>
#ifndef SCAN_H
#define SCAN_H

uint16_t my_htons(uint16_t hostshort);

uint32_t my_htonl(uint32_t hostshort);

int my_inet_pton(int af, const char *src, void *dst);

void *my_memset(void *s, int c, size_t n);

#endif
