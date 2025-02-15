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

void test_validate_double_when_out_of_upper_range(/* void **state */) {
    // ARRANGE
    double number;
    char err_message[100];

    // ACT
    bool result = validate_double("10.1\n", 0.0, 10.0, &number, err_message);

    // ASSERT
    assert_string_equal(err_message, "Entry must be greater than 0.00 and less than or equal to 10.00. Please try again.");
    assert_false(result);
}

void test_validate_double_when_out_of_lower_range(/* void **state */) {
    // ARRANGE
    double number;
    char err_message[100];

    // ACT
    bool result = validate_double("-0.1\n", 0.0, 10.0, &number, err_message);

    // ASSERT
    assert_string_equal(err_message, "Entry must be greater than 0.00 and less than or equal to 10.00. Please try again.");
    assert_false(result);
}

void test_validate_double_when_non_numeric() {
    // ARRANGE
    double input;
    char err_message[100];

    // ACT
    bool result = validate_double("a\n", 0.0, 10.0, &input, err_message);

    // ASSERT
    assert_string_equal(err_message, "Entry must be numeric. Please try again.");
    assert_false(result);
}

void test_validate_double_with_multiple_inputs() {
    // ARRANGE
    double number;
    char err_message[100];

    // ACT
    bool result = validate_double("5.5 6.5\n", 0.0, 10.0, &number, err_message);

    // ASSERT
    assert_string_equal(err_message, "Entry must be a single numeric value. Please try again.");
    assert_false(result);
}

void test_validate_double_with_multiple_non_numeric_inputs() {
    // ARRANGE
    double input;
    char err_message[100];

    // ACT
    bool result = validate_double("ab cd\n", 0.0, 10.0, &input, err_message);

    // ASSERT
    assert_string_equal(err_message, "Entry must be numeric. Please try again.");
    assert_false(result);
}

void test_validate_double_on_lower_boundary() {
    // ARRANGE
    double number;
    char err_message[100];

    // ACT
    bool result = validate_double("0.0\n", 0.0, 10.0, &number, err_message);

    // ASSERT
    assert_double_equal(number, 0.0, 0.001); 
    assert_true(result);
}

void test_validate_double_on_upper_boundary() {
    // ARRANGE
    double upper;
    char err_message[100];

    // ACT
    bool result = validate_double("10.0\n", 0.0, 10.0, &upper, err_message);

    // ASSERT
    assert_double_equal(upper, 10.0, 0.001); 
    assert_true(result);
}


int main(void) {
    const struct CMUnitTest tests[] = { // defines the list of tests to be ran
        cmocka_unit_test(test_validate_double_when_valid),
        cmocka_unit_test(test_validate_double_when_out_of_upper_range),
        cmocka_unit_test(test_validate_double_when_out_of_lower_range),
        cmocka_unit_test(test_validate_double_when_non_numeric),
        cmocka_unit_test(test_validate_double_with_multiple_inputs),
        cmocka_unit_test(test_validate_double_with_multiple_non_numeric_inputs),
        cmocka_unit_test(test_validate_double_on_lower_boundary),
        cmocka_unit_test(test_validate_double_on_upper_boundary)
    };

    // last two parameters: setup and takedown for the GROUP of tests
    return cmocka_run_group_tests(tests, NULL, NULL);
}