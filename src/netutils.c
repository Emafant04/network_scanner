#include <stdint.h>

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
