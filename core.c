/*
 * core.c:    Turing machine core
 *
 * Author:    Giorgio Pristia
 */

#include <stdlib.h>
#include "core.h"

int tm_run(struct tm *tm, struct tmconf *c){
    (void)tm;
    (void)c;
    struct queue *q = new_queue();
    if(!q) return -1;
    enqueue(q, c);
    delete_queue(q);
    return 2;
}

int tm_init(struct tm *tm){
    tm->rules = new_rule_dict();
    if(!tm->rules)
        return -1;
    tm->accept = new_set();
    if(!tm->accept){
        free(tm->rules);
        return -1;
    }
    return 0;
}

void tm_destroy(struct tm *tm){
    delete_rule_dict(tm->rules);
    delete_set(tm->accept);
}
