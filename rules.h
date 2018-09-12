/*
 * rules.h:   Rules dictionary functions
 *
 * Author:    Giorgio Pristia
 */

#ifndef RULES_H
#define RULES_H

#include "types.h"

struct rule{
    state  st_from,
           st_dest;
    symbol ch_from,
           ch_dest;
    int    mv_dest;
};

typedef struct rule_dest rule_dest;
typedef struct rule_dict rule_dict;

/* List of non deterministic destinations for each rule */
struct rule_dest{
    state       st;
    int         mv;
    symbol      ch;
    rule_dest  *next;
};

struct rule_dict{
    rule_dest *(*rule)[0x100];
    state      size;
};

/* Return the dictionary or NULL if malloc fails */
rule_dict *new_rule_dict   ();

/* Return 0 on success, else -1 */
int        rule_dict_insert(rule_dict*, struct rule*);
void       delete_rule_dict(rule_dict*);

#endif
