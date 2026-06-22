#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../src/netutils.h"

int scan_port(char *ip, int port){

    // 1. Crea il socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return PORT_ERROR;;
    }

    int flags = fcntl(sockfd, F_GETFL, 0);   // leggi i flag attuali
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); // aggiungi O_NONBLOCK

    // 2. Prepara l'indirizzo di destinazione
    struct sockaddr_in target;
    my_memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_port   = my_htons(port);

    if (my_inet_pton(AF_INET, ip, &target.sin_addr) <= 0) {
        fprintf(stderr, "Indirizzo IP non valido\n");
        close(sockfd);
        return PORT_ERROR;;
    }

    // 3. Tenta la connessione
    int result = connect(sockfd, (struct sockaddr *)&target, sizeof(target));
    return result;
}
