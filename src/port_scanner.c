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

    char *range = argv[2];
    char *ip = argv[1];
    int start_port=0;
    int end_port=0;

    int ports=parse_range(range,&start_port,&end_port);
    if(ports==0){
        int i = start_port;
        int j = end_port;
        while(i <= j){
            if(scan_port(ip,i)==PORT_OPEN){
                printf("%d PORTA APERTA\n",i);
            }
            i++;
        }
    }else{
        printf("errore nel parsing delle porte\n");
    }

    return 0;
}
