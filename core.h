/*
 * core.h:    Turing machine core
 *
 * Author:    Giorgio Pristia
 */
	
#ifndef CORE_H
#define CORE_H

#include "rules.h"
#include "accept.h"
#include "tape.h"
#include "queue.h"

/* Machine settings */
struct tm{
    rule_dict* rules;
    set *accept;
    unsigned int max;
};

int  tm_init   (struct tm*);
void tm_destroy(struct tm*);

#endif
