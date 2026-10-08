#ifndef PING_H
#define PING_H

#include <stdint.h>
#include <stddef.h>
#include "netutils.h"

typedef struct {
    uint8_t  type;
    uint8_t  code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
} icmp_header;

typedef struct {
    char **hosts;
    int count;
} host_list;

int ping_host(char *ip);
host_list ping_sweep(char *subnet, int cidr);
uint16_t icmp_checksum(const void *data, size_t len);

#endif
