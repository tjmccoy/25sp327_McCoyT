/* Private/internal queue attributes */

#include "queue.h"
#include <stdbool.h>

typedef struct queue_node {
    void* data;
    struct queue_node* next;
} queue_node_t; 

struct queue {
    queue_node_t* header;
    queue_node_t* tail;
    int size;
    bool isClosed;
    pthread_mutex_t* mutex;
    pthread_cond_t* cond_var;
};