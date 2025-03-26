#include <stdlib.h>
#include <sys/types.h>
extern char **environ;

int main(void) {
    char *fork_exec_env = getenv("FORK_EXEC_MODE");
    if (!fork_exec_env) {
        printf("Parent process (%d): Forking child...\n", getpid());
        pid_t ret;

        if ((ret = fork()) == -1) {
            perror("Fork failed.\n");
            exit(EXIT_FAILURE);
        }
        else if (ret == 0) {
            // child process
            printf("Child process (): Executing itself with FORK_EXEC_MODE");
            char *new_env[] = {
                "FORK_EXEC_MODE=1",
                NULL
            };
            if (execle("./fork_exec", "fork_exec", (char *) NULL) == -1) {
                perror("Execl failed.\n");
                return EXIT_FAILURE;
            }
        }
        else {
            // parent process
            wait(0);
            printf("Parent process (%d): Child (%d) has exited.\n");
        }
    } else {
        printf("Child fork_exec started (%d)\n", getpid());
        for (int i=0; environ[i] != NULL; ++i) {
            printf("\t%s\n", environ[i]);
        }
        pause();
        printf("\tChild (%d) Exiting\n", getpid());
    }
    return EXIT_SUCCESS;
}