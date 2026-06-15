#include <stdint.h>
#include <sys/socket.h>
#include <stddef.h>

uint16_t my_htons(uint16_t hostshort){
    uint8_t low = hostshort & 0x00FF;
    uint8_t high = (hostshort >> 8) & 0x00FF;

    uint16_t result = (low << 8) | high;
    return result;
}
uint32_t my_htonl(uint32_t hostshort){
    uint16_t low = hostshort & 0x0000FFFF;
    uint16_t high = (hostshort >> 16) & 0x0000FFFF;

    low=my_htons(low);
    high=my_htons(high);

    uint32_t result = ((uint32_t)low << 16) | high;
    return result;
}
int my_inet_pton(int af, const char *src, void *dst){
    if(src == NULL || dst == NULL) return -1;
    if(af==AF_INET){
        int countp=1;
        int value1=0;
        int value2=0;
        int value3=0;
        int value4=0;
        while(*src!='\0'){
            if(*src=='.'){
                countp++;
            }
            if('0'<=*src && *src<='9'){
                switch(countp){
                    case 1:value1 = value1 * 10 + (*src - '0');
                    break;
                    case 2:value2 = value2 * 10 + (*src - '0');
                    break;
                    case 3:value3 = value3 * 10 + (*src - '0');
                    break;
                    case 4:value4 = value4 * 10 + (*src - '0');
                    break;
                }
            }
            if(*src!='.'&& !('0'<=*src && *src<='9')){
                return 0;
            }
            src++;
        }
        if (countp != 4){
            return 0;
        }
        if(0<=value1 && value1<=255 &&0<=value2 && value2<=255 && 0<=value3 && value3<=255 && 0<=value4 && value4<=255){
            *(uint32_t *)dst=my_htonl(((uint32_t)value1 << 24) | ((uint32_t)value2 << 16) | ((uint32_t)value3 << 8) | (uint32_t)value4);
            return 1;
        }else{
            return 0;
        }
    }else{
        return-1;
    }
}

void *my_memset(void *s, int c, size_t n){
    size_t count=0;
    unsigned char *p = (unsigned char *)s;
    while(count<n){
        *p=(unsigned char)c;
        p++;
        count++;
    }
    return s;
}
