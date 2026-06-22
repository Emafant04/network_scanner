#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../src/scan.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <ip> <porta>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[2]);

    char *ip = argv[1];

    int result = scan_port(ip, port);

    if (result == 0) {
        printf("Porta %d aperta\n", port);
    } else {
        if (errno == ECONNREFUSED){
            printf("Porta %d chiusa\n", port);
        } else
          if(errno == EINPROGRESS){

          }
    }

    close(sockfd);
    return 0;
}
