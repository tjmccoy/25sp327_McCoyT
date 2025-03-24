#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* isprime(void* data);

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

    /* cast args as long long values*/
    number1 = atoll(argv[1]);
    number2 = atoll(argv[2]);

    /* initialize thread attributes & validate */
    pthread_attr_t th_attr;
    if (pthread_attr_init(&th_attr) != 0)
    {
        perror("pthread_attr_init");
        exit(EXIT_FAILURE);
    }

    /* thread declaration & initialization */
    pthread_t tid_prime1;
    pthread_t tid_prime2;
    pthread_create(&tid_prime1, &th_attr, isprime, &number1);
    pthread_create(&tid_prime2, &th_attr, isprime, &number2);

    /* have threads wait for their work to be done before terminating*/
    pthread_join(tid_prime1, NULL);
    pthread_join(tid_prime2, NULL);

    /* clean-up */
    pthread_attr_destroy(&th_attr);
    printf("Done!\n");

    return 0;
}

void* isprime(void *data)
{
    long long num = *((long long*) data);   // dereference data & store as a long long
    printf("isprime %lld\n", num);

    //TODO: write code to determine if num is prime & print result
    // LATER: return result & check return values w/ api call

    pthread_exit(NULL);
}