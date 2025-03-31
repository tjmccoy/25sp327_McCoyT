/* This is the public-facing header file. */

#include <pthread.h>
#include <stdio.h>

typedef struct queue queue_t;

extern queue_t* queue_init(pthread_mutex_t* mutex, pthread_cond_t* cond_var);

extern void queue_enqueue(queue_t* q);

extern void queue_dequeue(queue_t* q);