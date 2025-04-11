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

int __wrap_pthread_mutex_lock(pthread_mutex_t* mtx) {
    UNUSED(mtx);
    return mock_type(int);
}
int __wrap_pthread_mutex_unlock(pthread_mutex_t* mtx) {
    UNUSED(mtx);
    return mock_type(int);
}
// need to wrap pthread_cond_wait and pthread_cond_broadcast 

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
    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);

    // Act
    queue_enqueue(que, &data);

    // Assert
    assert_int_equal(que->size, 1);
    assert_ptr_equal(que->header->next, que->tail);
    assert_null(que->header->data);
    assert_non_null(que->tail);
    assert_non_null(que->tail->data);

    // Teardown
    free(que->tail);
    free(que->header);
    free(que);
}

void test_queue_destroy(void** state) {
    UNUSED(state);
    
    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    //queue_t* que = queue_init(&mutex, &cond_var);
    will_return(__wrap_pthread_mutex_lock, 0);
    will_return(__wrap_pthread_mutex_unlock, 0);
    

    // Act

    // Assert

    // Teardown
    
}

void test_queue_dequeue(void** state) {
    UNUSED(state);

    // Arrange
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    queue_t* q = queue_init(&mutex, &cond_var);
    data_t data;
    will_return_always(__wrap_pthread_mutex_lock, 0);
    will_return_always(__wrap_pthread_mutex_unlock, 0);

    // Act
    queue_enqueue(q, &data);
    assert_int_equal(q->size, 1);   // make sure queue had an element enqueued

    void* returned_val = queue_dequeue(q);

    // Assert
    assert_int_equal(q->size, 0);
    assert_ptr_equal(q->header, q->tail);
    void* null_val = queue_dequeue(q);
    assert_null(null_val);

    // Teardown
    free(returned_val);
    free(q->header);
    free(q);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_queue_init),
        cmocka_unit_test(test_queue_enqueue_when_q_empty),
        cmocka_unit_test(test_queue_dequeue)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}