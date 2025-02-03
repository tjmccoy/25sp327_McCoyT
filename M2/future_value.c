#include "future_value.h"

void print_usage(void) {
	fprintf(stderr, "%s", USAGE_MSG);
}

void parse_arguments(int argc, char *argv[], double *monthly_investment, double *annual_interest_rate, int *years) {
	char *endptr = NULL;
    long temp_long;
    double temp_double;

    /* check if exactly three arguments (excluding program name) are provided */
    if (argc != 4) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    /* parse monthly investment */
    errno = 0;
    temp_double = strtod(argv[1], &endptr);
    if (errno != 0 || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid input for monthly investment.\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }
    if (temp_double < 1 || temp_double > 1000) {
        fprintf(stderr, "Error: Monthly investment must be in the range [1, 1000].\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }
    *monthly_investment = temp_double;

    /* parse annual interest rate using strtod */
    errno = 0;
    temp_double = strtod(argv[2], &endptr);
    if (errno != 0 || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid input for annual interest rate.\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }
    if (temp_double < 1 || temp_double > 15) {
        fprintf(stderr, "Error: Annual interest rate must be in the range [1, 15].\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }
    *annual_interest_rate = temp_double;

    /* parse number of years using strtol */
    errno = 0;
    temp_long = strtol(argv[3], &endptr, 10);
    if (errno != 0 || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid input for number of years.\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }
    if (temp_long < 1 || temp_long > 50) {
        fprintf(stderr, "Error: Number of years must be in the range [1, 50].\n\n");
        print_usage();
        exit(EXIT_FAILURE);
    }
    *years = (int)temp_long;
}

void calculate_future_value(double monthly_investment, double annual_interest_rate, int years, double results[][2], double *final_future_value) {
    double monthly_interest_rate = annual_interest_rate / 12.0 / 100.0;
    int total_months = years * 12;
    double future_value = 0.0;
    double cumulative_interest = 0.0;
    int year_index = 0;

    for (int i = 0; i < total_months; i++) {
        // add the monthly investment
        future_value += monthly_investment;

        // calculate the interest for this month and add it to the future value
        double monthly_interest_amount = future_value * monthly_interest_rate;
        future_value += monthly_interest_amount;

        // accumulate the interest over all months so far
        cumulative_interest += monthly_interest_amount;

        // when a year is complete, store the cumulative interest ([year_index][0]) and future value ([year_index][1])
        if ((i + 1) % 12 == 0) {
            results[year_index][0] = cumulative_interest;
            results[year_index][1] = future_value;
            year_index++;
        }
    }
    *final_future_value = future_value;
}

void display_results(double monthly_investment, double annual_interest_rate, int years, double final_future_value, double results[][2]) {
    /* print the summary header */
    printf("Monthly Investment: %.2f\n", monthly_investment);
    printf("Yearly Interest Rate: %.2f%%\n", annual_interest_rate);
    printf("Years: %d\n\n", years);
    
    /* round the overall future value to the nearest whole number for summary */
    printf("Future value after %d years is $%.0f.\n\n", years, final_future_value);
    
    /* print the table header */
    printf("Year Accum Interest        Value\n");
    printf("==== ============== ============\n");
    
    /* print each year's details */
    for (int i = 0; i < years; i++) {
        printf("%4d %14.2f %12.2f\n", i + 1, results[i][0], results[i][1]);
    }
}

int main(int argc, char *argv[])
{
    /* initialize variables */
    double monthly_investment = 0.0;
    double annual_interest_rate = 0.0;
    int years = 0;
    double final_future_value = 0.0;

    /* parse and validate command line arguments */
    parse_arguments(argc, argv, &monthly_investment, &annual_interest_rate, &years);

    /* declare a 2d array to store results (per year)*/
    double results[years][2];

    /* calculate the future value and yearly accumulated interest */
    calculate_future_value(monthly_investment, annual_interest_rate, years, results, &final_future_value);

    /* display the input summary, overall future value and a table of results */
    display_results(monthly_investment, annual_interest_rate, years, final_future_value, results);

    return EXIT_SUCCESS;
}