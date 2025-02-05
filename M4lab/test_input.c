#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

bool validate_double(char* input, double low, double high, double* number, char* err_msg);

// our naming standard: "test_function_condition"

void test_validate_double_when_valid(/* void **state */) {
    // ARRANGE
    double number;
    char err_message[100];

    // ACT
    bool result = validate_double("5.5\n", 0.0, 10.0, &number, err_message);

    // ASSERT
    assert_double_equal(number, 5.5, 0.001);    // third param: epsilon
    assert_true(result);    // provided by cmocka unit testing framework
}

void test_validate_double_when_out_of_range(/* void **state */) {
    // ARRANGE
    double number;
    char err_message[100];

    // ACT
    bool result = validate_double("15.5\n", 0.0, 10.0, &number, err_message);

    // ASSERT
    assert_string_equal(err_message, "Entry must be greater than 0.00 and less than or equal to 10.00. Please try again.");
    assert_false(result);
}

int main(void) {
    const struct CMUnitTest tests[] = { // defines the list of tests to be ran
        cmocka_unit_test(test_validate_double_when_valid),
        cmocka_unit_test(test_validate_double_when_out_of_range)
    };

    // last two parameters: setup and takedown for the GROUP of tests
    return cmocka_run_group_tests(tests, NULL, NULL);
}