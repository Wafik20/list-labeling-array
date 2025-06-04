#include "lla.h"
#include <time.h>
#include <math.h>
#include <stdio.h>

double get_time_us()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000000.0) + (ts.tv_nsec / 1000.0);
}

double log2_n(double n)
{
    return log2(n);
}

int main()
{
    // Test parameters
    const int N = 10000000;
    const int C = 8;
    const double TAU_0 = 0.5;
    const double TAU_D = 0.75;
    const int NUM_INSERTIONS = 10000000;

    // Create LLA
    lla *my_lla = create_lla(N, C, TAU_0, TAU_D);

    // Arrays to store measurements
    double* times = malloc(NUM_INSERTIONS * sizeof(double));
    double* log2n_values = malloc(NUM_INSERTIONS * sizeof(double));

    printf("Testing insert() operation runtime complexity (vs log₂(n))...\n");
    printf("N = %d, C = %d, TAU_0 = %.2f, TAU_D = %.2f\n", N, C, TAU_0, TAU_D);
    printf("Performing %d insertions...\n\n", NUM_INSERTIONS);

    // Perform insertions and measure time
    for (int i = 0; i < NUM_INSERTIONS; i++)
    {
        int value = i + 1;

        double start_time = get_time_us();
        insert(my_lla, value);
        double end_time = get_time_us();

        times[i] = end_time - start_time;
        log2n_values[i] = log2_n(i + 1);

        if ((i + 1) % 100 == 0)
        {
            printf("Completed %d insertions\n", i + 1);
        }
    }

    // Calculate correlation coefficient with log2(n)
    double sum_xy = 0, sum_x = 0, sum_y = 0, sum_x2 = 0, sum_y2 = 0;
    for (int i = 0; i < NUM_INSERTIONS; i++)
    {
        sum_xy += times[i] * log2n_values[i];
        sum_x += times[i];
        sum_y += log2n_values[i];
        sum_x2 += times[i] * times[i];
        sum_y2 += log2n_values[i] * log2n_values[i];
    }

    double n = NUM_INSERTIONS;
    double correlation = (n * sum_xy - sum_x * sum_y) /
                         sqrt((n * sum_x2 - sum_x * sum_x) *
                              (n * sum_y2 - sum_y * sum_y));

    printf("\nResults:\n");
    printf("Correlation coefficient with log(n): %.4f\n", correlation);
    printf("Average insertion time: %.2f microseconds\n", sum_x / n);
    printf("Maximum insertion time: %.2f microseconds\n", times[NUM_INSERTIONS - 1]);
    printf("Minimum insertion time: %.2f microseconds\n", times[0]);

    // Cleanup
    free(times);
    free(log2n_values);
    cleanup_lla(&my_lla);
    return 0;
}
