/*
 * queue.h:   Configuration queue
 *
 * Author:    Giorgio Pristia
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"
#include "tape.h"

struct queue{
    struct tmconf *head;
    struct tmconf *tail;
};

struct tmconf{
    state          st;
    unsigned int   ttl;
    tape          *t;
    struct tmconf *next;
};

typedef struct queue queue;

void delete_tmconf(struct tmconf*);

queue         *new_queue   ();

static inline void enqueue(queue *q, struct tmconf *conf){
    q->head       = q->head ?
    q->head->next = conf    : (
    q->tail       = conf  ) ;
}

static inline struct tmconf *dequeue(queue *q){
    struct tmconf *conf;
    if((conf = q->tail)){
        q->tail = conf->next;
        conf->next = NULL;
    }
    if(!q->tail)
        q->head = NULL;
    return conf;
}

void           delete_queue(queue*);

#endif
