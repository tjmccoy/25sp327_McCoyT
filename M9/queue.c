#define _GNU_SOURCE
#include <stdlib.h>
#include "common.h"
#include "queue_internal.h"
#include "queue.h"

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

    LOCK_MTX(q->mutex);
    q->tail->next = new_node;
    q->tail = new_node;
    q->size += 1;
    UNLOCK_MTX(q->mutex);
}

void queue_destroy(queue_t* q) {
    free(q->header);
    free(q);
}

void queue_close(queue_t* q) {
    LOCK_MTX(q->mutex);
    q->isClosed = true;
    UNLOCK_MTX(q->mutex);
}

bool queue_isclose(queue_t* q) {
    bool closed = false;
    LOCK_MTX(q->mutex);
    closed = q -> isClosed;
    UNLOCK_MTX(q->mutex);

    return closed;
}

void* queue_dequeue(queue_t* q) {
    if (!q) return NULL;    // case 0: queue doesn't exist

    LOCK_MTX(q->mutex); // begin work in critical section
    if (q->header == q->tail) {
        UNLOCK_MTX(q->mutex);
        return NULL;
    }

    queue_node_t* old_head = q->header; // pointer manipulation
    void* data = old_head->data;
    q->header = old_head->next;
    q->size -= 1;

    if (!q->header) {   // if queue is now empty, update tail
        q->tail = NULL;
    }
    free(old_head); // free allocated memory
    UNLOCK_MTX(q->mutex);   // critical section work is done, unlock the mutex

    return data;
}