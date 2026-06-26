#ifndef SCAN_H
#define SCAN_H
#include "netutils.h"

#define PORT_OPEN     0
#define PORT_CLOSED   1
#define PORT_FILTERED 2
#define PORT_ERROR    3

int scan_port(char *ip, int port);

int parse_range(char *range, int *start, int *end);

#endif
