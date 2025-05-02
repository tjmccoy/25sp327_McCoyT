#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "common.h"
#include "http_proxy.h"
#include "queue.h"
#include "thread_pool.h"

#define LISTEN_PORT 8080  // Proxy listens on this port

static volatile sig_atomic_t keep_running = 1;  // for sigint_handler (below)

static void sigint_handler(int sig)
{
    (void)sig;
    keep_running = 0;   // controls main loop
}

int main(int argc, char* argv[]) {
    // Accept command-line arguments for the number of threads
    int num_threads = 2;    // provide a default number of threads
    if (argc == 2) {
        num_threads = atoi(argv[1]);
    }

    // Initialize a queue and thread pool to handle requests
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  cv  = PTHREAD_COND_INITIALIZER;

    // Create queue and thread pool
    queue_t *que = queue_init(&mtx, &cv);
    struct thread_pool *pool = thread_pool_init(que, num_threads);
    
    // Create socket for the proxy server to listen on the LISTEN_PORT
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
        handle_error("socket");

    int opt = 1;    // allow for rebinding (SO_REUSEADDR)
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        handle_error("setsockopt");

    // bind to localhost:8080
    struct sockaddr_in addr;  
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(LISTEN_PORT);

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        handle_error("bind");

    // start listening  
    if (listen(listen_fd, 32) == -1)
        handle_error("listen");

    // Add a signal handler to cleaning shutdown the server
    // when a SIGINT (ctrl-c) is received.
    signal(SIGPIPE, SIG_IGN);   // ignore sigpipe

    // handle SIGINT gracefully
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    printf("http_proxy listening on port %d with %d worker threads …\n", LISTEN_PORT, num_threads);

    // Accept incoming client connections. Add a request_t to the queue to
    // handle the client's HTTP request. The client socket is the argument to be
    // passed to the handle_client_request_thread function.
    // Continue to accept client connections until SIGINT is received.
    while (keep_running) {
        struct sockaddr_storage cli_addr;
        socklen_t cli_len = sizeof(cli_addr);

        int *client_fd = malloc(sizeof(int));
        if (!client_fd)
            handle_error("malloc");

        *client_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &cli_len);
        if (*client_fd == -1) {
            free(client_fd);
            if (errno == EINTR && !keep_running)
                break; // interrupted by SIGINT
            perror("accept"); 
            continue;   // keep listening
        }
        
        // wrap ptr in request_t obj and enqueue
        request_t *req = malloc(sizeof(request_t));
        if (!req)  
            handle_error("malloc");

        req->work_fn = handle_client_request_thread;
        req->arg     = client_fd;

        queue_enqueue(que, req);
    }


    // perform clean up
    queue_close(que);
    thread_pool_destroy(pool);
    queue_destroy(que);

    close(listen_fd);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cv);

    return EXIT_SUCCESS;
}