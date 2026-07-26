#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "../src/scan.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <ip> <porta>\n", argv[0]);
        return 1;
    }

    char *range = argv[2];
    char *ip = argv[1];
    int start_port=0;
    int end_port=0;

    struct timespec start, end;

    int ports=parse_range(range,&start_port,&end_port);
    if(ports==0){
        int i = start_port;
        int j = end_port;

        pthread_mutex_t mutex;
        sem_t semaphore;
        pthread_mutex_init(&mutex, NULL);
        sem_init(&semaphore, 0, 50);

        int num_ports = j - i + 1;
        pthread_t *threads = malloc(num_ports * sizeof(pthread_t));
        thread_args *args = malloc(num_ports * sizeof(thread_args));

        clock_gettime(CLOCK_MONOTONIC, &start);
        int idx = 0;
        while(i <= j){
            sem_wait(&semaphore);
            args[idx].ip = ip;
            args[idx].port = i;
            args[idx].mutex = &mutex;
            args[idx].semaphore = &semaphore;
            pthread_create(&threads[idx], NULL, scan_thread, &args[idx]);
            idx++;
            i++;
        }
        for(int k = 0; k < num_ports; k++){
            pthread_join(threads[k], NULL);
        }
        pthread_mutex_destroy(&mutex);
        sem_destroy(&semaphore);
        free(threads);
        free(args);

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Time: %.3f seconds\n", elapsed);
        fflush(stdout);
    }else{
        printf("parsing range error\n");
    }

    return 0;
}
