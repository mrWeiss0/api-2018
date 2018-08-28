/*
 * tape.c:    Machine tape data structures
 *
 * Author:    Giorgio Pristia
 */

#ifndef TAPE_H
#define TAPE_H

#include "types.h"

typedef struct tape tape;

/* Initialize a new tape containing the string passed */
tape  *tape_init  (symbol*);
/* Branch the current tape and return a pointer to the new copy created */
tape  *tape_branch(tape*);
symbol tape_read  (tape*);
tape  *tape_write (tape*, symbol, int move);
void   delete_tape(tape*);

#endif
