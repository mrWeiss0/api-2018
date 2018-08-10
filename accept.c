/*
 * accept.c:  Accept states set
 *
 * Author:    Giorgio Pristia
 *
 * Accepting states are stored in a set with a function
 * to know if a state is an accepting state or not.
 * The set is first implemented as a list of states that is scanned
 * on each access, but if it grows large enough it is changed
 * in a bitfield that uses a single bit to store if a state is
 * present or not in the set. The maximum state value is needed to
 * calculate the bitfield size and the number of elements needed
 * to turn from list to bitfield.
 */

#include <stdlib.h>
#include "accept.h"

struct set{
    enum {list, bits}  format;
    size_t             max;
    size_t             count;
    void              *data;
};

typedef struct state_list{
    state              st;
    struct state_list *next;
} state_list;

typedef void* put_funct(void*, state);
typedef int   get_funct(void*, state);

put_funct bits_put, list_put, * const f_put[] = {list_put, bits_put};
get_funct bits_get, list_get, * const f_get[] = {list_get, bits_get};

void delete_list(state_list*);

set* new_set(){
    set* s    = malloc(sizeof(*s));
    s->format = list;
    s->max    =
    s->count  = 0;
    s->data   = NULL;
    return s;
}

int set_put(set* s, state st){
    void *t = f_put[s->format](s->data, st);
    if(t){
        s->count++;
        s->data = t;
    }
    return !!t;
}

int set_get(set* s, state st){
    return f_get[s->format](s->data, st);
}

void set_max(set *s, size_t max){
    if(s->max < max)
        s->max = max;
}

void delete_set(set* s){
    if(s->format == list)
        delete_list(s->data);
    else
        free(s->data);
    free(s);
}

void *bits_put(void *bits, state st){
    ((char*) bits)[st >> 3] |= 1 << (st & 7);
    return bits;
}

int bits_get(void *bits, state st){
    return !!(((char*) bits)[st >> 3] & 1 << (st & 7));
}

void *list_put(void *list, state st){
    state_list* t = malloc(sizeof(*t));
    t->st         = st;
    t->next       = list;
    return t;
}

int list_get(void *list, state st){
    for(; list; list = ((state_list*)list)->next)
        if(((state_list*)list)->st == st)
            return 1;
    return 0;
}

void delete_list(state_list* list){
    state_list *t;
    while(list){
        t = list->next;
        free(list);
        list = t;
    }
}
