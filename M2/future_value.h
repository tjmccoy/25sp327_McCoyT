#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* Usage message definition */
#define USAGE_MSG "Usage: ./future_value <monthly-investment> <annual-interest-rate> <number-of-years>\n" \
"  monthly-investment: numeric value in range [1, 1000] representing the amount invested each month.\n" \
"annual-interest-rate: decimal value in range [1, 15] representing the annual interest rate.\n" \
"     number-of-years: integer value in range [1, 50] representing the duration of the investment in years.\n"

// Function Prototypes
void print_usage(void);
void parse_arguments(int argc, char *argv[], double *monthly_investing, double *annual_interest_rate, int *years);
void calculate_future_value(double monthly_investment, double annual_interest_rate, int years, double results[][2], double *final_future_value);
void display_results(double monthly_investment, double annual_interest_rate, int years, double final_future_value, double results[][2]);
