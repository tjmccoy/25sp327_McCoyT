#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define handle_error(msg)                                        \
    do                                                           \
    {                                                            \
        int errnum = errno;                                      \
        fprintf(stderr, "%s: (%s) %s\n",                         \
                msg, strerrorname_np(errnum), strerror(errnum)); \
        exit(EXIT_FAILURE);                                      \
    } while (0)

#define handle_errnum(errnum, msg)                               \
    do                                                           \
    {                                                            \
        fprintf(stderr, "%s: (%s) %s\n",                         \
                msg, strerrorname_np(errnum), strerror(errnum)); \
        exit(EXIT_FAILURE);                                      \
    } while (0)