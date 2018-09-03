/*
 * queue.c:   Configuration queue
 *
 * Author:    Giorgio Pristia
 *
 * This queue is used to store configurations in BFS
 * (dequeue) tail => => => head (enqueue)
 */

#include <stdlib.h>
#include "queue.h"

struct queue{
    struct tmconf *head;
    struct tmconf *tail;
};

struct queue *new_queue(tape *t){
    struct queue  *q    = calloc(1, sizeof(*q));
    if(!q) return NULL;
    struct tmconf *conf = calloc(1, sizeof(*conf));
    if(!conf){
        free(q);
        return NULL;
    }
    conf->t = t;
    enqueue(q, conf);
    return q;
}

void enqueue(struct queue *q, struct tmconf *conf){
    q->head       = q->head ?
    q->head->next = conf    : (
    q->tail       = conf  ) ;
}

struct tmconf *dequeue(struct queue *q){
    struct tmconf *conf;
    if((conf = q->tail))
        q->tail = conf->next;
    if(!q->tail)
        q->head = NULL;
    return conf;
}

void delete_queue(struct queue *q){
    struct tmconf *conf;
    while((conf = dequeue(q))){
        delete_tape(conf->t);
        free(conf);
    }
    free(q);
}
