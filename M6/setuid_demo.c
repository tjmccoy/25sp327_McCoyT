#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/fsuid.h>
#include <stdlib.h>

// macro definition to print UIDS
#define PRINT_UIDS(tag) do { \
    uid_t ruid, euid, suid; \
    if (getresuid(&ruid, &euid, &suid) == 0) { \
        printf("%s: RUID=%d, EUID=%d, SUID=%d\n", tag, ruid, euid, suid); \
    } else { \
        perror("getresuid failed"); \
    } \
} while (0)

int main() {
    // t0
    PRINT_UIDS("t0: Running with root privileges"); 

    // t1
    if (seteuid(getuid()) == -1) {
        perror("seteuid failed");
        exit(EXIT_FAILURE);
    }
    PRINT_UIDS("t1: Dropped privileges temporarily");
    
    // t2
    if (seteuid(0) == -1) {
        perror("seteuid restore failed");
        exit(EXIT_FAILURE);
    }
    PRINT_UIDS("t2: Restored privileges");

    // t3
    if (seteuid(getuid()) == -1) {
        perror("seteuid failed");
        exit(EXIT_FAILURE);
    }
    PRINT_UIDS("t3: Dropped privileges again");

    // t4
    printf("t4: Permanently dropping privileges...\n");
    if (setresuid(1000, 1000, 1000) == -1) { // Drop to UID 1000 (non-root user)
        perror("setresuid failed");
        exit(EXIT_FAILURE);
    }
    PRINT_UIDS("t4: After permanent drop");

    // t5
    printf("t5: Attempting to restore privileges...\n");
    if (seteuid(0) == -1) { // Attempt to restore to root privileges
        perror("t5: Failed to restore privileges");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}