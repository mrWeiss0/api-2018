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
    state          st;
    unsigned int   ttl;
    tape          *t;
    struct tmconf *next;
};

typedef struct queue queue;

queue         *new_queue   ();
void           enqueue     (queue*, struct tmconf*);
struct tmconf *dequeue     (queue*);
void           delete_queue(queue*);

#endif
