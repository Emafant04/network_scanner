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
        clock_gettime(CLOCK_MONOTONIC, &start);
        while(i <= j){
            int status = scan_port(ip, i);
            if(status == PORT_OPEN){
                printf("%d PORT OPEN\n", i);
            } else if(status == PORT_FILTERED){
                printf("%d PORT FILTERED\n", i);
            }
            i++;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Time: %.3f seconds\n", elapsed);
        fflush(stdout);
    }else{
        printf("parsing range error\n");
    }

    return 0;
}
