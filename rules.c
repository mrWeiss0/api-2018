/*
 * rules.c:   Rules dictionary functions
 *
 * Author:    Giorgio Pristia
 *
 * The dictionary stores each rule in a hash table.
 * Keys consist of the current state and read symbol,
 * values are lists of destinations.
 * When a rule is added, if a rule from that configuration
 * already exists, the new non deterministic destination is pushed
 * in the correct list of the dictionary. Otherwise a new element
 * is inserted in the dictionary with a single destination in its list.
 */

#include <stdlib.h>
#include "rules.h"

#define DICT_MINSZ 8

typedef unsigned int     hash;
typedef struct rule_list rule_list;

struct rule_dict{
    rule_list **rule;
    size_t      size,
                count;
};

struct rule_list{
    hash        hash;
    state       st;
    symbol      ch;
    rule_dest  *dest;
    rule_list  *next;
};


hash hash_f(state st, symbol ch){
    return st + ch;
}

rule_list *rule_list_find  (rule_list*, state, symbol);

/* Return:  0 on success
 *         -1 if malloc fails on new rule
 *         -2 if rule_dest_push call fails on existing rule
 */
int        rule_list_insert(rule_list**, hash, struct rule*);
void       delete_rule_list(rule_list*);

/* Return 0 on success, else -1 */
int        rule_dest_push  (rule_dest**, state, char, symbol);
void       delete_rule_dest(rule_dest*);

/******************** Dictionary ********************/

rule_dict *new_rule_dict(){
    rule_dict *dict = malloc(sizeof(*dict));
    if(!dict)
        return NULL;
    dict->rule  = calloc(DICT_MINSZ, sizeof(*dict->rule));
    if(!dict->rule){
        free(dict);
        return NULL;
    }
    dict->size  = DICT_MINSZ;
    dict->count = 0;
    return dict;
}

rule_dest *rule_dict_find(rule_dict *dict, state st, symbol ch){
    hash h = hash_f(st, ch);
    rule_list *r =
        rule_list_find(dict->rule[h % dict->size], st, ch);
    return r ? r->dest : NULL;
}

int rule_dict_insert(rule_dict *dict, struct rule *rule){
    hash h = hash_f(rule->st_from, rule->ch_from);
    /*
     * Increase dictionary count only when a new rule is inserted
     * and not when a new destination is added to an existing rule
     */
    int t = rule_list_insert(dict->rule + h % dict->size, h, rule);
    if(t < 0)
        return -1;
    dict->count += t;
    return 0;
}

void delete_rule_dict(rule_dict *dict){
    size_t i;
    for(i = 0; i < dict->size; i++)
        delete_rule_list(dict->rule[i]);
    free(dict->rule);
    free(dict);
}

/******************** List ********************/

rule_list *rule_list_find(rule_list *rule, state st, symbol ch){
    for(; rule; rule = rule->next)
        if(rule->st == st &&
           rule->ch == ch)
            return rule;
    return NULL;
}

int rule_list_insert(rule_list **list, hash h, struct rule *rule){
    int        incr = 0;
    rule_list *new  = rule_list_find(*list, rule->st_from, rule->ch_from);
    if(!new){
        new       = malloc(sizeof(*new));
        if(!new)
            return -1;
        new->hash = h;
        new->st   = rule->st_from;
        new->ch   = rule->ch_from;
        new->dest = NULL;
        new->next = *list;
        *list     = new;
        incr      = 1;
    }
    if(rule_dest_push(&new->dest, rule->st_dest,
                      rule->mv_dest, rule->ch_dest) < 0)
        return -2;
    return incr;
}

void delete_rule_list(rule_list *list){
    rule_list *t;
    while(list){
        t = list->next;
        delete_rule_dest(list->dest);
        free(list);
        list = t;
    }
}

/******************** Destination ********************/

int rule_dest_push(rule_dest **dest, state st, char mv, symbol ch){
    rule_dest *new = malloc(sizeof(*new));
    if(!new)
        return -1;
    new->next = *dest;
    new->st   = st;
    new->mv   = mv;
    new->ch   = ch;
    *dest     = new;
    return 0;
}

void delete_rule_dest(rule_dest *dest){
     rule_dest *t;
     while(dest){
        t = dest->next;
        free(dest);
        dest = t;
    }
}
