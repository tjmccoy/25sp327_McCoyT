#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t tstp_received = 0;
volatile sig_atomic_t usr2_received = 0;
volatile sig_atomic_t term_received = 0;

int running_sum = 0;

void sigtstp_handler(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)info;
    (void)context;

    tstp_received = 1;
}

void sigusr2_handler(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)context;

    if (info->si_pid == getppid())
        usr2_received = 1;
}

void sigterm_handler(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)info;
    (void)context;

    term_received = 1;
}

int main(void) {
    struct sigaction sa;

    sa.sa_sigaction = sigtstp_handler;
    sa.sa_flags = SA_SIGINFO;

    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        perror("sigaction(SIGTSTP)");
        exit(EXIT_FAILURE);
    }

    sa.sa_sigaction = sigusr2_handler;
    sa.sa_flags = SA_SIGINFO;

    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("sigaction(SIGUSR2)");
        exit(EXIT_FAILURE);
    }

    sa.sa_sigaction = sigterm_handler;
    sa.sa_flags = SA_SIGINFO;

    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction(SIGTERM)");
        exit(EXIT_FAILURE);
    }

    while (1) {
        running_sum += 10;

        printf("Child: Running... Use 'kill -SIGTSTP %d' to interrupt me.\n", getpid());
        sleep(1);

        if (tstp_received) {
            tstp_received = 0;

            printf("Child: SIGTSTP received. Sending sum to parent.\n");

            union sigval value;
            value.sival_int = running_sum;

            if (sigqueue(getppid(), SIGUSR1, value) == -1) {
                perror("sigqueue(SIGUSR1)");
            }
        }

        if (usr2_received) {
            usr2_received = 0;

            printf("Child: Received SIGUSR2 from Parent (PID: %d). Current sum = %d\n", getppid(), running_sum);
        }

        if (term_received) {
            printf("Child: Received SIGTERM, exiting...\n");

            union sigval value;
            value.sival_int = running_sum;

            if (sigqueue(getppid(), SIGUSR1, value) == -1) {
                perror("sigqueue(final SIGUSR1)");
            }
            break;
        }
    }
    return 0;
}
