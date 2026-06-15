#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../src/netutils.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <ip> <porta>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[2]);

    // 1. Crea il socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // 2. Prepara l'indirizzo di destinazione
    struct sockaddr_in target;
    my_memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_port   = my_htons(port);

    if (my_inet_pton(AF_INET, argv[1], &target.sin_addr) <= 0) {
        fprintf(stderr, "Indirizzo IP non valido\n");
        close(sockfd);
        return 1;
    }

    // 3. Tenta la connessione
    int result = connect(sockfd, (struct sockaddr *)&target, sizeof(target));

    if (result == 0) {
        printf("Porta %d aperta\n", port);
    } else {
        if (errno == ECONNREFUSED)
            printf("Porta %d chiusa\n", port);
        else
            perror("connect");
    }

    close(sockfd);
    return 0;
}
