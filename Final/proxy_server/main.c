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

int main(int argc, char* argv[]) {
    // Accept command-line arguments for the number of threads
    int num_threads = 2;    // provide a default number of threads
    if (argc == 2) {
        num_threads = atoi(argv[1]);
    }

    // TODO Initialize a queue and thread pool to handle requests

    // TODO Create socket for the proxy server to listen on the LISTEN_PORT

    // TODO Add a signal handler to cleaning shutdown the server
    // when a SIGINT (ctrl-c) is received.


    // TODO Accept incoming client connections. Add a request_t to the queue to
    // handle the client's HTTP request. The client socket is the argument to be
    // passed to the handle_client_request_thread function.
    // Continue to accept client connections until SIGINT is received.


    // TODO perform any clean up before server shutdown

    return EXIT_SUCCESS;
}