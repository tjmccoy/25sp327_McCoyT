#include <setjmp.h>
#include <stddef.h>
#include <string.h>
/* cmocka.h must be included after all other includes*/
#include <cmocka.h>
#include "queue_internal.h"

#define UNUSED(x) (void)(x)

void test_queue_init(void** state) {
    UNUSED(state);

    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    // Act
    queue_t* q = queue_init(&mutex, &cond_var);

    // Assert
    assert_non_null(q);
    assert_ptr_equal(q->mutex, &mutex);
}
