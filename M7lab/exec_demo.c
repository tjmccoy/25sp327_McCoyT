#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) 
{
    printf("PID is %d\n", getpid());
    printf("Hit enter to continue...");

    getchar();

    printf("Now executing less command\n");

    if (execl("/usr/bin/less", "less", "/etc/passwd", (char *) NULL) == -1) {
        perror("Execl failed.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
