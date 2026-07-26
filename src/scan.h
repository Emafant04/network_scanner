#ifndef SCAN_H
#define SCAN_H
#include <pthread.h>
#include <semaphore.h>
#include "netutils.h"

#define PORT_OPEN     0
#define PORT_CLOSED   1
#define PORT_FILTERED 2
#define PORT_ERROR    3

typedef struct {
    char *ip;
    int port;
    pthread_mutex_t *mutex;
    sem_t *semaphore;
} thread_args;

int scan_port(char *ip, int port);

int parse_range(char *range, int *start, int *end);

void *scan_thread(void *arg);

#endif
