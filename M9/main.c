#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"
#include "queue.h"
#include "tasks.h"
#include "thread_pool.h"

static request_t* create_request(int task_type);
static void random_sleep_time_in_range(int min_ms, int max_ms, struct timespec* ts);
static void nap_random();

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Accept command-line arguments for:
    // - number of worker threads
    // - total number of requests to generate
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_threads> <num_requests>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_threads = atoi(argv[1]);
    int num_requests = atoi(argv[2]);

    // seed random number to vary results between program executions
    srand(time(NULL));

    // initialize mutex and cond variables
    pthread_mutex_t req_mutex;
    pthread_cond_t req_cond;
    pthread_mutex_init(&req_mutex, NULL);
    pthread_cond_init(&req_cond, NULL);

    // initialize queue and thread pool
    queue_t* que = queue_init(&req_mutex, &req_cond);
    struct thread_pool* thread_pool = thread_pool_init(que, num_threads);

    int count = 0;
    while (count <= num_requests) {
        int task_type = rand() % 3;
        queue_enqueue(que, create_request(task_type));
        nap_random();
        count++;
    }

    queue_close(que);
    thread_pool_destroy(thread_pool);
    queue_destroy(que);

    // cleanup mutex and cond var
    pthread_mutex_destroy(&req_mutex);
    pthread_cond_destroy(&req_cond);

    pthread_exit((void*)0);
}

/// @brief Sleep for random milliseconds in range [50, 500]
static void nap_random() {
    struct timespec sleep_time;
    random_sleep_time_in_range(50, 500, &sleep_time);
    nanosleep(&sleep_time, NULL);
}

/// @brief a struct timespec with a random sleep time in the given range.
/// @param min_ms Minimum number of milliseconds (inclusive)
/// @param max_ms Maximum number of milliseconds (inclusive)
/// @param ts Pointer to struct timespec to be filled
static void random_sleep_time_in_range(int min_ms, int max_ms, struct timespec* ts) {
    if (min_ms > max_ms || min_ms < 0) {
        // Invalid range, default to 0
        min_ms = max_ms = 0;
    }

    int millis = min_ms + rand() % (max_ms - min_ms + 1);

    ts->tv_sec = millis / 1000;
    ts->tv_nsec = (millis % 1000) * 1000000L;
}

/// @brief Creates a request of the given task_type
/// @param task_type 
/// @return a populated request
static request_t* create_request(int task_type) {
    static int msg_count = 1;
    request_t* r = malloc(sizeof(request_t));
    switch (task_type) {
        case 0:
            char* msg = malloc(64);
            snprintf(msg, 64, "Log message %d", msg_count++);
            r->work_fn = write_log;
            r->arg = msg;
            return r;

        case 1:
            int* val = malloc(sizeof(int));
            *val = rand() % 1000;
            r->work_fn = compute_sum;
            r->arg = val;
            return r;

        case 2:
            struct timespec* delay = malloc(sizeof(struct timespec));
            random_sleep_time_in_range(25, 250, delay);
            r->work_fn = delay_task;
            r->arg = delay;
            return r;

        default:
            free(r);
            fprintf(stderr, "Unknown task type %d.", task_type);
            return NULL;
    }
}
