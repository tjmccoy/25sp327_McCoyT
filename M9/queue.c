#include <stdlib.h>
#include "common.h"
#include "queue_internal.h"
#include "queue.h"  // should NOT have to include queue.h b/c internal should bring it in. FIX THIS

queue_t* queue_init(pthread_mutex_t* mutex, pthread_cond_t* cond_var) {
    // allocate memory for the queue
    queue_t* q = malloc(sizeof(queue_t));

    // error handling for queue initialization
    if (!q) {
        perror("queue_init");
        exit(EXIT_FAILURE);
    }

    // intialize attributes with default values
    q->mutex = mutex;
    q->cond_var = cond_var;
    q->isClosed = false;
    q->size = 0;

    q->header = calloc(1, sizeof(queue_node_t));

    // error handling for header initialization
    if (!q->header) {
        perror("queue_init:header");
        exit(EXIT_FAILURE);
    }

    // tail = header by default
    q->tail = q->header;

    // return initialized queue
    return q;
}

void queue_enqueue(queue_t* q, void* data) {
    queue_node_t* new_node = malloc(sizeof(queue_node_t));

    if (!q) {
        handle_error("queue_enqueue:malloc");
    }

    new_node->data = data;
    new_node->next = NULL;

    // BEGIN CS
    q->tail->next = new_node;
    q->tail = new_node;
    q->size += 1;

    // END CS
}