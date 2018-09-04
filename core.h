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
    rule_dict    *rules;
    set          *accept;
    unsigned int  max;
};

int  tm_init   (struct tm*);

/*
 * Takes a tm and a starting configuration: tape, state and time to live
 * Returns the resulting state of the machine
 * Return  0: not accept
 *         1: accept
 *         2: non terminating
 *        -1: memory error
 */
int  tm_run    (struct tm*, struct tmconf*);
void tm_destroy(struct tm*);

#endif
