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

void delete_tmconf(struct tmconf *conf){
        delete_tape(conf->t);
        free(conf);
}

queue *new_queue(){
    return calloc(1, sizeof(queue));
}

void delete_queue(queue *q){
    struct tmconf *conf;
    while((conf = dequeue(q)))
        delete_tmconf(conf);
    free(q);
}
