/*
 * bits.c:    Bitfield
 *
 * Author:    Giorgio Pristia
 */

#include "bits.h"

char *bits_set(char *bits, size_t i){
    bits[i >> 3] |= 1 << (i & 7);
    return bits;
}

char *bits_unset(char *bits, size_t i){
    bits[i >> 3] &= ~(1 << (i & 7));
    return bits;
}

int bits_get(char *bits, size_t i){
    return !!(bits[i >> 3] & 1 << (i & 7));
}

size_t bits_next_avail(char *bits, size_t len, size_t start){
    size_t i, n;
    for(i = start >> 3, n = start & 7; i < BYTES(len); i++, n = 0)
        if(bits[i] != -1)
            for(;(                        /* for each bit in byte          */
                    !(len & 7)         ||
                    i < BYTES(len) - 1 || /* if last byte, the bits beyond */
                    n < (len & 7)         /* group len % 8 do not count    */
               ) && n < 8; n++)
                if (!(bits[i] >> n & 1))  /* if bit is 0, i.e. available,  */
                    return (i << 3) + n;  /* return its index in bitfield  */
    return len;
}
