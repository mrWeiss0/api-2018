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
#include "bits.h"

struct set{
    enum {list, bits}  format;
    state              max;
    size_t             count;
    void              *data;
};

typedef struct state_list{
    state              st;
    struct state_list *next;
} state_list;

/* Format dependent functions */
typedef void *put_funct(void*, state);
typedef int   get_funct(void*, state);
typedef void  del_funct(void*);
put_funct list_put, * const f_put[] = {list_put, (put_funct*)bits_set};
get_funct list_get, * const f_get[] = {list_get, (get_funct*)bits_get};
del_funct delete_list,        * const f_del[] = {delete_list, free};

char *list_to_bits(void**, size_t);

/******************** Set ********************/

set *new_set(){
    set *s    = malloc(sizeof(*s));
    if(!s) return NULL;
    s->format = list;
    s->max    =
    s->count  = 0;
    s->data   = NULL;
    return s;
}

int set_put(set* s, state st){
    size_t bits_sz = BYTES(s->max + 1);
    /* if list is larger than bits convert to bitfield */
    if(s->format == list && sizeof(state_list) * (s->count + 1) >= bits_sz){
        if(list_to_bits(&s->data, bits_sz))
            s->format = bits;
    }
    void *t = f_put[s->format](s->data, st);
    if(t){
        s->count++;
        s->data = t;
    }
    return !!t;
}

int set_get(set *s, state st){
    return f_get[s->format](s->data, st);
}

void set_max(set *s, state max){
    if(s->max < max)
        s->max = max;
}

void delete_set(set *s){
    f_del[s->format](s->data);
    free(s);
}

/******************** List ********************/

void *list_put(void *list, state st){
    state_list* t = malloc(sizeof(*t)); 
    if(!t) return NULL;
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

char *list_to_bits(void **list, size_t size){
    char *bits = calloc(size, sizeof(*bits));
    if(!bits)
        return NULL;
    state_list* l;
    for(l = *(state_list**)list; l; l = l->next)
        bits_set(bits, l->st);
    delete_list(*list);
    return *list = bits;
}

void delete_list(void *list){
    state_list *t;
    while(list){
        t = ((state_list*)list)->next;
        free(list);
        list = t;
    }
}
