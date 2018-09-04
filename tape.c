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
#include "bits.h"
#include "tape.h"

#define TAPEGR_SZ 256

struct tgroup{
    symbol cell[TAPEGR_SZ];
    struct tgroup *link[2]; /* 0: prev, 1: next */
};

struct tmeta{
    char   active[BYTES(TAPEGR_SZ)];
    size_t count;
    size_t next_avail;
};

struct tape{
    struct tmeta  *meta;
    struct tgroup *head;
    size_t         idx;
};

/*
 * Set (unset) used tapes in a group
 * In(de)crease group count
 * Update next available index
 */
void tmeta_set(struct tmeta* meta, size_t i){
    bits_set(meta->active, i);
    ++meta->count;
    meta->next_avail = bits_next_avail(meta->active, TAPEGR_SZ, i + 1);
}

void tmeta_unset(struct tmeta* meta, size_t i){
    bits_unset(meta->active, i);
    --meta->count;
    if(i < meta->next_avail)
        meta->next_avail = i;
}

/*
 * If t is NULL or its group is full create a new empty tape in a new group
 * otherwise place a new tape in the first available index of the group
 * with head pointing to the same head of t
 */
tape* new_tape(tape *t){
    tape *tnew = malloc(sizeof(*tnew));
    if(!tnew) return NULL;
    if(!(t && t->meta->count < TAPEGR_SZ)){
        if(!( (tnew->head = calloc(1, sizeof(*tnew->head))) &&
              (tnew->meta = calloc(1, sizeof(*tnew->meta))) ))
            return NULL;
        tnew->idx  = 0;
    }
    else{
        tnew->head = t->head;
        tnew->meta = t->meta;
        tnew->idx  = t->meta->next_avail;
    }
    tmeta_set(tnew->meta,tnew->idx);
    return tnew;
}

symbol tape_read(tape *t){
    return t->head->cell[t->idx];
}

tape *tape_write(tape *t, symbol write, int move){
    int dir;
    t->head->cell[t->idx] = write;
    while(move){                 /* move tape head of specified cells */
        dir = move > 0;
        move -= (dir * 2 - 1);   /* Decrease move absolute value      */
        if(!t->head->link[dir]){ /* Create new cells if necessary     */
            if(!(t->head->link[dir] = calloc(1, sizeof(*t->head))))
                return NULL;
            t->head->link[dir]->link[!dir] = t->head;
        }
        t->head = t->head->link[dir];
    }
    return t;
}

tape *tape_branch(tape *t){
    tape *tnew = new_tape(t);
    if(t && tnew){
        struct tgroup *i, *j, *k;
        for(int v = 0; v <= 1; v++){ /* Copy tape in two directions */
            i = t->head;
            j = k = tnew->head;
            if(!v){                  /* Skip head in first pass     */
                k = k->link[v];
                i = i->link[v];
            }
            while(i){
                if(!k){
                    /* Create new cells if the new tape is in a new group */
                    if(!(k = calloc(1, sizeof(*k)))){
                        delete_tape(t);
                        return NULL;
                    }
                    k->link[!v] = j;
                    j->link[v] = k;
                }
                k->cell[tnew->idx] = i->cell[t->idx];
                j = k;
                k = j->link[v];
                i = i->link[v];
            }
        }
    }
    return tnew;
}

tape *tape_init(symbol *s, symbol blank, symbol term){
    tape *t          = tape_branch(NULL);
    struct tgroup *j = t->head;
    if(*s && *s != term && t) for(;;){
        j->cell[t->idx] = *s == blank ? '\0' : *s;
        if(!*++s || *s == term) break;
        if(!(j->link[1] = calloc(1, sizeof(*j)))){
            delete_tape(t);
            return NULL;
        }
        j->link[1]->link[0] = j;
        j = j->link[1];
    }
    return t;
}

void delete_tgroup(struct tgroup *tg){
    struct tgroup *i, *j;
    for(int v = 0; v <= 1; v++){ /* Delete in two directions from head */
        i = tg;
        if(!v)                   /* Skip head in first pass            */
            i = i->link[v];
        while(i){
            j = i->link[v];
            free(i);
            i = j;
        }
    }
}

void delete_tape(tape *t){
    tmeta_unset(t->meta, t->idx);
    if(!t->meta->count){ /* Delete group when its last tape is deleted */
        delete_tgroup(t->head);
        free(t->meta);
    }
    free(t);
}
