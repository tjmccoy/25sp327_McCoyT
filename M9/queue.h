/* This is the public-facing header file. */

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct queue queue_t;

/// @brief Create a queue with the given mutex and condition variables.
/// @param mutex used to synchronize access to critical sections
/// @param cond_var 
/// @return queue
extern queue_t* queue_init(pthread_mutex_t* mutex, pthread_cond_t* cond_var);

/// @brief Enqueue an element onto the queue.
/// @param q queue data structure
/// @param data item to be enqueued
extern void queue_enqueue(queue_t* q, void* data);

/// @brief 
/// @param q 
extern void queue_dequeue(queue_t* q);

/// @brief 
/// @param q 
extern void queue_close(queue_t* q);

/// @brief 
/// @param q 
/// @return 
extern int queue_size(queue_t* q);

/// @brief 
/// @param q 
/// @return 
extern bool queue_isClosed(queue_t* q);

/// @brief 
/// @param q 
extern void queue_destroy(queue_t* q);