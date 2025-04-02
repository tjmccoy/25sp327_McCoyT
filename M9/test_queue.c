#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
/* cmocka.h must be included after all other includes*/
#include <cmocka.h>
#include "queue_internal.h"

#define UNUSED(x) (void)(x)

typedef struct {
    int x;
} data_t;

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
    assert_ptr_equal(q->cond_var, &cond_var);
    assert_false(q->isClosed);
    assert_int_equal(q->size, 0);
    assert_non_null(q->header);

    // Cleanup
    free(q->header);
    free(q);
}

void test_queue_enqueue_when_q_empty(void** state) {
    UNUSED(state);

    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    queue_t* q = queue_init(&mutex, &cond_var);
    data_t data;

    // Act
    queue_enqueue(q, &data);

    // Assert
    assert_int_equal(q->size, 1);

    // Cleanup
    free(q->header);
    free(q);
}
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_queue_init),
        cmocka_unit_test(test_queue_enqueue_when_q_empty)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}