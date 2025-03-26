#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define HANDLE_ERROR(errnum, msg) \
    do { \
        fprintf(stderr, "%s: (%s) %s\n", msg, strerrorname_np(errnum), strerror(errnum)); \
        exit(EXIT_FAILURE); \
    } while (0)

void* isprime(void* data);
void* progress(void* data);

int main(int argc, char* argv[]) 
{
    long long number1;
    long long number2;

    /* input validation */
    if (argc != 3) 
    {
        fprintf(stderr, "Enter two integer values.\n");
        exit(EXIT_FAILURE);
    }

    /* cast args as long long values */
    number1 = atoll(argv[1]);
    number2 = atoll(argv[2]);

    /* initialize thread attributes & validate */
    int rval;
    pthread_attr_t th_attr;

    if ((rval = pthread_attr_init(&th_attr)) != 0) 
    {
        HANDLE_ERROR(rval, "pthread_attr_init");
    }
    if ((rval = pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_JOINABLE)) != 0) 
    {
        HANDLE_ERROR(rval, "pthread_attr_setdetachstate");
    }

    /* thread declaration */
    pthread_t tid_prime1;
    pthread_t tid_prime2;
    pthread_t tid_progress;

    /* thread initialization and error handling */
    if ((rval = pthread_create(&tid_prime1, &th_attr, isprime, &number1))) {
        HANDLE_ERROR(rval, "pthread_create isprime1");
    }
    if ((rval = pthread_create(&tid_prime2, &th_attr, isprime, &number2))) {
        HANDLE_ERROR(rval, "pthread_create isprime2");
    }
    if ((rval = pthread_create(&tid_progress, &th_attr, progress, NULL))) {
        HANDLE_ERROR(rval, "pthread_create progress");
    }

    /* have threads wait for their work to be done before terminating */
    pthread_detach(tid_progress);
    pthread_join(tid_prime1, NULL);
    pthread_join(tid_prime2, NULL);

    //sleep(3); observe threads using htop

    /* clean-up */
    if ((rval = pthread_cancel(tid_progress)) != 0) {
        HANDLE_ERROR(rval, "pthread_cancel");
    }
    pthread_attr_destroy(&th_attr);
    //sleep(5); observe threads using htop
    printf("Done!\n");

    return 0;
}

void* isprime(void *data)
{
    long long num = *((long long*) data);   // dereference data & store as a long long
    int prime = 1;

    for (long long j = 2; j < num; j++) 
    {
        if (num % j == 0) { prime = 0; }
    }
    printf("\n%lld is %s.\n", num, prime ? "prime" : "composite");
    pthread_exit(NULL);
}

void* progress(void* data) {
    while(1) 
    {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    pthread_exit(NULL);
}