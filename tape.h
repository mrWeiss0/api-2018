/*
 * tape.c:    Machine tape data structures
 *
 * Author:    Giorgio Pristia
 */

#ifndef TAPE_H
#define TAPE_H

#include <string.h>
#include "types.h"

struct tape{
    symbol *tail[2];
    size_t  size[2];
    long    head;
};

typedef struct tape tape;

/*
 * Initialize a new tape containing the string passed
 * blank are replaced with zeroes and term, if present,
 * marks the end of the string (to ignore trailing \n)
 */
tape  *tape_init  (symbol*, symbol blank, symbol *term);
/* Branch the current tape and return a pointer to the new copy created */
tape  *tape_branch(tape*);

static inline symbol tape_read(tape *t){
    if(t->head < 0) return t->tail[0][~t->head];
    return t->tail[1][t->head];
}

static inline tape *tape_write(tape *t, symbol write, int move){
    if(t->head < 0) t->tail[0][~t->head] = write;
    else t->tail[1][t->head] = write;
    t->head += move;
    if(t->head >= (signed)t->size[1]){
        t->tail[1] = realloc(t->tail[1], (t->head + 1) * sizeof(**t->tail));
        memset(t->tail[1] + t->size[1], 0, move);
        t->size[1] = t->head + 1;
    }
    else if(t->head < -(signed)t->size[0]){
        t->tail[0] = realloc(t->tail[0], -t->head * sizeof(**t->tail));
        memset(t->tail[0] + t->size[0], 0, -move);
        t->size[0] = -t->head;
    }
    return t;
}
void   delete_tape(tape*);

#endif
