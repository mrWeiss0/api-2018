/*
 * core.c:    Turing machine core
 *
 * Author:    Giorgio Pristia
 */

#include <stdlib.h>
#include "core.h"

int tm_run(struct tm *tm, struct tmconf *c){
    rule_dest *d;
    struct tmconf *c_;
    int o = 0;
    struct queue *q = new_queue();
    if(!q){
        delete_tmconf(c);
        return -1;
    }
    enqueue(q, c);
    /* Loop until reache accept state or queue is empty */
    while(!(o & 1) && (c = dequeue(q))){
        /* Outgoing transitions from the current configuration */
        d = rule_dict_find(tm->rules, c->st, tape_read(c->t));
        if(!d)                 /* No outgoing transitions: dead branch    */
            delete_tmconf(c);
        else if(!c->ttl){      /* There are outgoing transitions but time */
            delete_tmconf(c);  /* to live is zero: non terminating branch */
            o = 2;
        }
        else for(; d; d = d->next){
            if(set_get(tm->accept, d->st)){
                delete_tmconf(c); /* As soon as an accepting state is */
                o = 1;            /* reached, TM stops and returns 1  */
                break;
            }
            if(d->next){
                if(!(c_ = calloc(1, sizeof(*c_)))){
                    delete_tmconf(c);
                    o = -1;
                    break;
                }
                c_->t = tape_branch(c->t); /* Branch current configuration */
            }
            else /* Last transition is applied inplace without branching   */
                c_ = c;
            /* Apply transition and enqueue the configuration reached */
            c_->st = d->st;
            c_->ttl = c->ttl - 1;
            tape_write(c_->t, d->ch, d->mv);
            enqueue(q, c_);
        };
    }
    /* Clear all remaining enqueued configurations and return */
    delete_queue(q);
    return o;
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
