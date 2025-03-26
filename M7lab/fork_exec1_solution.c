#include <stdlib.h>
#include <sys/types.h>
#define MAX_CHILDREN 3

extern char **environ;

int main(void) {
    char *fork_exec_env = get_env("FORK_EXEC_MODE");
    if (!fork_exec_env) {
        for (int i = 1; i <= MAX_CHILDREN; ++i) {
            printf("Creating child process (%d)...\n", i);
        }
    }
}

void exec_self() {
    // Child process
    printf("Child process (%d): Executing itself with FORK_EXEC_MODE set.\n", getpid());

}

void do_child_work() {
    // Child processing is done here
    printf("Child fork_exec started (%d)\n", getpid());
    for (int i = 0; environ[i] != NULL; ++i) {
        printf("\t%s\n", environ[i]);
    }
    pause();
    printf("\tChild (%d) exiting\n", getpid());
}

void create_child() {

}

void signal_handler(int signum) {
    // write to stdout
}