#include <malloc.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

// cmocka.h must be included after all other includes
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
    queue_t* que = queue_init(&mutex, &cond_var);

    // Assert
    assert_non_null(que);
    assert_ptr_equal(que->mutex, &mutex);
    assert_ptr_equal(que->cond_var, &cond_var);
    assert_false(que->isClosed);
    assert_int_equal(que->size, 0);
    assert_non_null(que->header);
    assert_ptr_equal(que->header, que->tail);

    // cleanup
    free(que->header);
    free(que);
}

void test_queue_enqueue_when_q_empty(void** state) {
    UNUSED(state);
    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    queue_t* que = queue_init(&mutex, &cond_var);
    data_t data;

    // Act
    queue_enqueue(que, &data);

    // Assert
    assert_int_equal(que->size, 1);
    // TODO add asserts to verify links are correct.
    // TODO add asserts to verify data is set in tail node.

}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_queue_init),
        cmocka_unit_test(test_queue_enqueue_when_q_empty)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}