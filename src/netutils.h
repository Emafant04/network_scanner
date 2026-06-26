#include <stdint.h>
#include <stddef.h>
#ifndef NETUTILS_H
#define NETUTILS_H

uint16_t my_htons(uint16_t hostshort);

uint32_t my_htonl(uint32_t hostshort);

int my_inet_pton(int af, const char *src, void *dst);

void *my_memset(void *s, int c, size_t n);

#endif
