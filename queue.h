/*
 * queue.h:   Configuration queue
 *
 * Author:    Giorgio Pristia
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"
#include "tape.h"

struct tmconf{
    state st;
    unsigned int count;
    tape *t;
    struct tmconf *next;
};

/* Create a queue with the initial configuration */
struct queue  *new_queue   (tape *t);
void           enqueue     (struct queue*, struct tmconf*);
struct tmconf *dequeue     (struct queue*);
void           delete_queue(struct queue*);

#endif
