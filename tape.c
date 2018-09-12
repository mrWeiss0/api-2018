/*
 * tape.c:    Machine tape data structures
 *
 * Author:    Giorgio Pristia
 *
 * Tapes are stored in groups, each group is a double linked list of arrays
 * The nth element of the array is the cell of the nth tape of the group in
 * the pointed position. New tapes are added to the group until it is full,
 * then a new group is created. When a group is empty, it is destroyed.
 * All tapes from a group link to the same metadata relative to
 * used and available indices in the group
 *
 * t3:  _ .... a .... _
 * t2:  b .... _ .... _
 * t1:  a .... a .... _
 * t0:  a .... b .... _
 *      ^      ^      ^
 *  <- c0 <-> c1 <-> c2 ->
 */

#include <stdlib.h>
#include "tape.h"

tape *tape_branch(tape *t){
    tape *tnew = calloc(1, sizeof(*tnew));
    if(t && tnew) for(int i = 0; i <= 1; i++){
        if(!(tnew->tail[i] = malloc(t->size[i] * sizeof(**tnew->tail)))){
            free(*tnew->tail);
            free(tnew);
            return NULL;
        }
        memcpy(tnew->tail[i], t->tail[i], t->size[i]);
        tnew->size[i] = t->size[i];
        tnew->head = t->head;
    }
    return tnew;
}

tape *tape_init(symbol *s, symbol blank, symbol *term){
    tape *t  = tape_branch(NULL);
    if(!t) return NULL; 
    t->size[1] = strcspn(s, term);
    t->tail[1] = calloc(t->size[1], sizeof(**t->tail));
    for(size_t l = 0; s[l] && s[l] != *term; l++)
        if(s[l] != blank)
            t->tail[1][l] = s[l];
    return t;
}

void delete_tape(tape *t){
    for(int i = 0; i <= 1; i++)
        free(t->tail[i]);
    free(t);
}
