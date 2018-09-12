/*
 * rules.h:   Rules dictionary functions
 *
 * Author:    Giorgio Pristia
 */

#ifndef RULES_H
#define RULES_H

#include "types.h"

typedef unsigned int     hash;

struct rule{
    state  st_from,
           st_dest;
    symbol ch_from,
           ch_dest;
    int    mv_dest;
};

typedef struct rule_dict rule_dict;
typedef struct rule_list rule_list;
typedef struct rule_dest rule_dest;

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

/* List of non deterministic destinations for each rule */
struct rule_dest{
    state       st;
    int         mv;
    symbol      ch;
    rule_dest  *next;
};

static inline hash hash_f(state st, symbol ch){
    return st * 5 + ch * 3;
}

/* Return the dictionary or NULL if malloc fails */
rule_dict *new_rule_dict   ();

static inline rule_list *rule_list_find(rule_list *rule, state st, symbol ch){
    for(; rule; rule = rule->next)
        if(rule->st == st &&
           rule->ch == ch)
            return rule;
    return NULL;
}

static inline rule_dest *rule_dict_find(rule_dict *dict, state st, symbol ch){
    hash h = hash_f(st, ch);
    rule_list *r =
        rule_list_find(dict->rule[h % dict->size], st, ch);
    return r ? r->dest : NULL;
}

/* Return 0 on success, else -1 */
int        rule_dict_insert(rule_dict*, struct rule*);
void       delete_rule_dict(rule_dict*);

#endif
