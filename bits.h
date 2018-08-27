/*
 * bits.h:    Bitfield
 *
 * Author:    Giorgio Pristia
 */

#ifndef BITS_H
#define BITS_H

#include <stdlib.h>

#define BYTES(BITS) (((BITS) >> 3) + !!((BITS) & 7))

char *bits_set   (char*, size_t);
char *bits_unset (char*, size_t);
char *bits_toggle(char*, size_t);
int   bits_get   (char*, size_t);

/* Return next available index in bitfield or -1 if the it's full */
size_t bits_next_avail(char*, size_t len, size_t start);

#endif
