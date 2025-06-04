#include "lla.h"
#include <time.h>
#include <math.h>

double get_time_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000000.0) + (ts.tv_nsec / 1000.0);
}

double log2_squared(double n) {
    double log2n = log2(n);
    return log2n * log2n;
}

int main() {
    // Test parameters
    const int N = 1024;  // Array size
    const int C = 8;     // Capacity factor
    const double TAU_0 = 0.5;
    const double TAU_D = 0.75;
    const int NUM_INSERTIONS = 100;  // Number of insertions to test

    // Create LLA
    lla *my_lla = create_lla(N, C, TAU_0, TAU_D);

    // Arrays to store measurements
    double times[NUM_INSERTIONS];
    double log2n_squared[NUM_INSERTIONS];

    printf("Testing insert() operation runtime complexity...\n");
    printf("N = %d, C = %d, TAU_0 = %.2f, TAU_D = %.2f\n", N, C, TAU_0, TAU_D);
    printf("Performing %d insertions...\n\n", NUM_INSERTIONS);

    // Perform insertions and measure time
    for (int i = 0; i < NUM_INSERTIONS; i++) {
        int value = i + 1;  // Insert values 1 to NUM_INSERTIONS
        
        double start_time = get_time_us();
        insert(my_lla, value);
        double end_time = get_time_us();
        
        times[i] = end_time - start_time;
        log2n_squared[i] = log2_squared(i + 1);

        // Print progress every 100 insertions
        if ((i + 1) % 100 == 0) {
            printf("Completed %d insertions\n", i + 1);
        }
    }

    // Calculate correlation coefficient
    double sum_xy = 0, sum_x = 0, sum_y = 0, sum_x2 = 0, sum_y2 = 0;
    for (int i = 0; i < NUM_INSERTIONS; i++) {
        sum_xy += times[i] * log2n_squared[i];
        sum_x += times[i];
        sum_y += log2n_squared[i];
        sum_x2 += times[i] * times[i];
        sum_y2 += log2n_squared[i] * log2n_squared[i];
    }

    double n = NUM_INSERTIONS;
    double correlation = (n * sum_xy - sum_x * sum_y) / 
                        sqrt((n * sum_x2 - sum_x * sum_x) * 
                             (n * sum_y2 - sum_y * sum_y));

    printf("\nResults:\n");
    printf("Correlation coefficient with log²(n): %.4f\n", correlation);
    printf("Average insertion time: %.2f microseconds\n", sum_x / n);
    printf("Maximum insertion time: %.2f microseconds\n", times[NUM_INSERTIONS-1]);
    printf("Minimum insertion time: %.2f microseconds\n", times[0]);

    // Cleanup
    cleanup_lla(&my_lla);
    return 0;
}
