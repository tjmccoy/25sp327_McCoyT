#define _GNU_SOURCE

#include <errno.h>
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define handle_error(errnum, msg)                                \
    do {                                                         \
        fprintf(stderr, "%s: (%s) %s\n",                         \
                msg, strerrorname_np(errnum), strerror(errnum)); \
        exit(EXIT_FAILURE);                                      \
    } while (0)

struct prime_result {
    int prime;
    long long factor1;
    long long factor2;
};

void *isprime(void *data);
void *progress(void *data);
void print_result(long long num, struct prime_result* result);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Enter two integer values.\n");
        exit(EXIT_FAILURE);
    }
    long long number1;
    long long number2;

    number1 = atoll(argv[1]);
    number2 = atoll(argv[2]);

    int rval;
    pthread_attr_t th_attr;
    if ((rval = pthread_attr_init(&th_attr)) != 0) {
        handle_error(rval, "pthead_attr_init");
    }
    if ((rval = pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_JOINABLE)) != 0) {
        handle_error(rval, "pthead_attr_setdetachstate");
    }

    pthread_t tid_prime1;
    pthread_t tid_prime2;
    pthread_t tid_progress;
    if ((rval = pthread_create(&tid_prime1, &th_attr, isprime, &number1)) != 0) {
        handle_error(rval, "pthead_create isprime1");
    }
    if ((rval = pthread_create(&tid_prime2, &th_attr, isprime, &number2)) != 0) {
        handle_error(rval, "pthead_create isprime2");
    }
    if ((rval = pthread_create(&tid_progress, &th_attr, progress, NULL)) != 0) {
        handle_error(rval, "pthead_create progress");
    }

    pthread_detach(tid_progress);

    struct prime_result *prime1;
    struct prime_result *prime2;

    if ((rval = pthread_join(tid_prime1, (void*) &prime1)) != 0) {
        handle_error(rval, "pthread_join:prime1");
    }
    if ((rval = pthread_join(tid_prime2, (void*) &prime2)) != 0) {
        handle_error(rval, "pthread_join:prime2");
    }

    if ((rval = pthread_cancel(tid_progress)) != 0) {
        handle_error(rval, "pthread_cancel");
    }
    pthread_attr_destroy(&th_attr);

    print_result(number1, prime1);
    print_result(number2, prime2);

    free(prime1);
    free(prime2);
    
    printf("Done!\n");
    pthread_exit(NULL);
}

void print_result(long long num, struct prime_result* result) {
    if (result->prime) {
        printf("\n%lld is prime.\n", num);
    } else {
        printf("\n%lld is composite. Factors found are %lld and %lld.\n", num, result->factor1, result->factor2);
    }
}

void *isprime(void *data) {
    struct prime_result *result = calloc(1, sizeof(struct prime_result));
    long long num = *((long long *)data);
    result->prime = 1;

    for (long long j = 2; j < num; j++) {
        if (num % j == 0) {
            result->prime = 0;
            result->factor1 = j;
            result->factor2 = num / j;
            break;  // comment out to simulate long running cpu-bound process.
        }
    }

    pthread_exit((void*)result);
}

void *progress(void *arg) {
    (void)arg;
    while (1) {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    pthread_exit(NULL);
}