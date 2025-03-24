#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t usr1_received = 0;
volatile sig_atomic_t chld_received = 0;
volatile sig_atomic_t child_sum = 0;
pid_t child_pid = 0;

void sigusr1_handler(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)context;

    if (info->si_pid == child_pid) {
        child_sum = info->si_value.sival_int;
        usr1_received = 1;
    }
}

void sigchld_handler(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)info;
    (void)context;

    chld_received = 1;
}

int main(void) {
    struct sigaction sa;
    int status;

    sa.sa_sigaction = sigusr1_handler;
    sa.sa_flags = SA_SIGINFO;

    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction(SIGUSR1)");
        exit(EXIT_FAILURE);
    }

    sa.sa_sigaction = sigchld_handler;
    sa.sa_flags = SA_SIGINFO | SA_NOCLDSTOP;

    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction(SIGCHLD)");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();

    if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        if (execl("./child", "child", (char *)NULL) == -1) {
            perror("execl");
            exit(EXIT_FAILURE);
        }
    }

    printf("Parent: fork-exec successful. Child pid (%d)\n", child_pid);

    while (1) {
        sleep(3);
        printf("Parent: Working...\n");

        if (usr1_received) {
            printf("Parent: Received SIGUSR1 from Child (PID: %d). Sum = %d\n", child_pid, child_sum);
            
            usr1_received = 0;
            if (kill(child_pid, SIGUSR2) == -1) {
                perror("kill(SIGUSR2)");
            }
        }

        if (chld_received) {
            if (waitpid(child_pid, &status, 0) == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            printf("Parent: Child (PID: %d) has terminated with status %d. Exiting.\n",
                   child_pid, WEXITSTATUS(status));
            break;
        }
    }
    return 0;
}
