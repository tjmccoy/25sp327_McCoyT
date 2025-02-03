#include <stdio.h>
#include <malloc.h>
#include "mymalloc.h"


int main(void) {

    printf("malloc_stats test driver\n");
    malloc_stats();

    char *ptr = alloc_bytes(512);
    printf("\nWe allocated 512 bytes %p\n", ptr);
    malloc_stats();

    printf("\nMalloc stats after call to free(%p)\n", ptr);

    free(ptr);
    ptr = NULL;

    malloc_stats();

    return 0;
}