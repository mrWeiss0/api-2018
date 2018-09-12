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
 * Hash table grows twice larger when the dictionary is 3/4 full.
 *
 * (state, symbol) => [(state, move, symbol) -> (state, move, symbol) -> ]
 */

#include <stdlib.h>
#include <string.h>
#include "rules.h"

/* Return 0 on success, else -1 */
int        rule_dest_push  (rule_dest**, state, int, symbol);
void       delete_rule_dest(rule_dest*);

/******************** Dictionary ********************/

rule_dict *new_rule_dict(){
    rule_dict *dict = calloc(1, sizeof(*dict));
    if(!dict)
        return NULL;
    return dict;
}

void rule_dict_grow(rule_dict *dict, state new){
    dict->rule = realloc(dict->rule, new * sizeof(*dict->rule));
    memset(dict->rule + dict->size, 0, (new - dict->size) * sizeof(*dict->rule));
    dict->size = new;
}

int rule_dict_insert(rule_dict *dict, struct rule *rule){
    state max = (rule->st_from > rule->st_dest ? rule->st_from : rule->st_dest) + 1;
    if(max > dict->size)
        rule_dict_grow(dict, max);
    rule_dest_push(dict->rule[rule->st_from] + rule->ch_from,
                   rule->st_dest, rule->mv_dest, rule->ch_dest);
    return 0;
}

void delete_rule_dict(rule_dict *dict){
    for(state i = 0; i < dict->size; i++)
    for(int j = 0; j < 0x100; j++)
        delete_rule_dest(dict->rule[i][j]);
    free(dict->rule);
    free(dict);
}

/******************** Destination ********************/

int rule_dest_push(rule_dest **dest, state st, int mv, symbol ch){
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
