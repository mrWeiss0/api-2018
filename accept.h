/*
 * accept.h:  Accept states set
 *
 * Author:    Giorgio Pristia
 */

#ifndef ACCEPT_H
#define ACCEPT_H

#include <stdlib.h>
#include "types.h"

typedef struct set set;

set* new_set();
int  set_put(set*, state);
int  set_get(set*, state);        /* True if the state is stored in the set */
void set_max(set *s, size_t max); /* Set the maximum element in the set     */
void delete_set(set*);

#endif
