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

queue *new_queue(){
    return calloc(1, sizeof(queue));
}

void enqueue(queue *q, struct tmconf *conf){
    q->head       = q->head ?
    q->head->next = conf    : (
    q->tail       = conf  ) ;
}

struct tmconf *dequeue(queue *q){
    struct tmconf *conf;
    if((conf = q->tail))
        q->tail = conf->next;
    if(!q->tail)
        q->head = NULL;
    return conf;
}

void delete_queue(queue *q){
    struct tmconf *conf;
    while((conf = dequeue(q))){
        delete_tape(conf->t);
        free(conf);
    }
    free(q);
}
