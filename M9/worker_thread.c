#include "worker_thread.h"
#include <string.h>
#include "common.h"

void* do_work(void* worker_thread_params) {
    struct worker_thread_params* params = (struct worker_thread_params*)worker_thread_params;
    fprintf(stderr, "thread-%d starting\n", params->thread_id);

    int req_count = 0;
    request_t* req = NULL;

    while (1) {
        req = queue_dequeue(params->que);

        if (req == NULL) {
            if (queue_isclose(params->que)) {
                break;
            }
            continue;
        }

        req->work_fn(req->arg);
        free(req);
        req_count++;
    }

    fprintf(stderr, "thread-%d exiting. Processed %d requests.\n", params->thread_id, req_count);
    free(params);
    pthread_exit((void*)0);
    return NULL;
}

pthread_t create_worker_thread(struct worker_thread_params* params) {
    pthread_t tid;
    int err = pthread_create(&tid, NULL, do_work, (void*)params);
    if (err != 0) {
        handle_error_en(err, "pthread_create");
    }
    return tid;
}