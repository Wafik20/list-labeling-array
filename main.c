#include "lla.h"
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double get_time_us()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000000.0) + (ts.tv_nsec / 1000.0);
}

// Proper complexity testing function
void test_complexity_at_size(int size, int num_trials, double *avg_time, double *log_size, double *log2_size)
{
    const int N = 1000000; // Large enough to accommodate all tests
    const int C = 8;
    const double TAU_0 = 0.5;
    const double TAU_D = 0.75;
    
    double total_time = 0.0;
    
    for (int trial = 0; trial < num_trials; trial++)
    {
        // Create fresh LLA for each trial
        lla *test_lla = create_lla(N, C, TAU_0, TAU_D);
        
        // Fill with 'size' elements first
        for (int i = 0; i < size; i++)
        {
            insert(test_lla, rand() % (size * 10)); // Random values
        }
        
        // Now measure insertion time into structure of size 'size'
        int test_value = rand() % (size * 10);
        
        double start_time = get_time_us();
        insert(test_lla, test_value);
        double end_time = get_time_us();
        
        total_time += (end_time - start_time);
        
        cleanup_lla(&test_lla);
    }
    
    *avg_time = total_time / num_trials;
    *log_size = log2((double)size);
    *log2_size = (*log_size) * (*log_size); // log²(n)
}

int main()
{
    srand(time(NULL));
    
    // Test different sizes
    int sizes[] = {100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int trials_per_size = 50; // Average over multiple trials
    
    double *avg_times = malloc(num_sizes * sizeof(double));
    double *log_sizes = malloc(num_sizes * sizeof(double));
    double *log2_sizes = malloc(num_sizes * sizeof(double));
    
    printf("Testing insert() complexity with proper methodology...\n");
    printf("Measuring insertion time vs structure size (averaged over %d trials each)\n\n", trials_per_size);
    
    for (int i = 0; i < num_sizes; i++)
    {
        printf("Testing size %d... ", sizes[i]);
        fflush(stdout);
        
        test_complexity_at_size(sizes[i], trials_per_size, &avg_times[i], &log_sizes[i], &log2_sizes[i]);
        
        printf("avg time: %.2f μs\n", avg_times[i]);
    }
    
    // Calculate correlation with log(n)
    double sum_xy_log = 0, sum_x_log = 0, sum_y_log = 0, sum_x2_log = 0, sum_y2_log = 0;
    // Calculate correlation with log²(n)
    double sum_xy_log2 = 0, sum_x_log2 = 0, sum_y_log2 = 0, sum_x2_log2 = 0, sum_y2_log2 = 0;
    
    for (int i = 0; i < num_sizes; i++)
    {
        // For log(n) correlation
        sum_xy_log += avg_times[i] * log_sizes[i];
        sum_x_log += avg_times[i];
        sum_y_log += log_sizes[i];
        sum_x2_log += avg_times[i] * avg_times[i];
        sum_y2_log += log_sizes[i] * log_sizes[i];
        
        // For log²(n) correlation
        sum_xy_log2 += avg_times[i] * log2_sizes[i];
        sum_x_log2 += avg_times[i];
        sum_y_log2 += log2_sizes[i];
        sum_x2_log2 += avg_times[i] * avg_times[i];
        sum_y2_log2 += log2_sizes[i] * log2_sizes[i];
    }
    
    double n = num_sizes;
    double correlation_log = (n * sum_xy_log - sum_x_log * sum_y_log) /
                             sqrt((n * sum_x2_log - sum_x_log * sum_x_log) *
                                  (n * sum_y2_log - sum_y_log * sum_y_log));
    
    double correlation_log2 = (n * sum_xy_log2 - sum_x_log2 * sum_y_log2) /
                              sqrt((n * sum_x2_log2 - sum_x_log2 * sum_x_log2) *
                                   (n * sum_y2_log2 - sum_y_log2 * sum_y_log2));
    
    printf("\n=== RESULTS ===\n");
    printf("Correlation with log(n):  %.4f\n", correlation_log);
    printf("Correlation with log²(n): %.4f\n", correlation_log2);
    
    // Print detailed results
    printf("\nSize\tlog₂(n)\tlog²₂(n)\tTime (μs)\tTime/log(n)\tTime/log²(n)\n");
    printf("----\t-------\t--------\t---------\t-----------\t------------\n");
    for (int i = 0; i < num_sizes; i++)
    {
        double ratio_log = avg_times[i] / log_sizes[i];
        double ratio_log2 = avg_times[i] / log2_sizes[i];
        printf("%d\t%.2f\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n", 
               sizes[i], log_sizes[i], log2_sizes[i], avg_times[i], ratio_log, ratio_log2);
    }
    
    // Interpretation
    printf("\nInterpretation:\n");
    printf("LOG(N) ANALYSIS:\n");
    if (correlation_log > 0.9)
        printf("✓ EXCELLENT: Strong log(n) behavior (correlation > 0.9)\n");
    else if (correlation_log > 0.7)
        printf("✓ GOOD: Reasonable log(n) behavior (correlation > 0.7)\n");
    else if (correlation_log > 0.5)
        printf("⚠ MODERATE: Weak log(n) behavior (correlation > 0.5)\n");
    else
        printf("✗ POOR: Does not follow log(n) behavior (correlation < 0.5)\n");
    
    printf("\nLOG²(N) ANALYSIS:\n");
    if (correlation_log2 > 0.9)
        printf("✓ EXCELLENT: Strong log²(n) behavior (correlation > 0.9)\n");
    else if (correlation_log2 > 0.7)
        printf("✓ GOOD: Reasonable log²(n) behavior (correlation > 0.7)\n");
    else if (correlation_log2 > 0.5)
        printf("⚠ MODERATE: Weak log²(n) behavior (correlation > 0.5)\n");
    else
        printf("✗ POOR: Does not follow log²(n) behavior (correlation < 0.5)\n");
    
    // Compare which correlation is stronger
    printf("\nBEST FIT:\n");
    if (correlation_log2 > correlation_log + 0.05) {
        printf("✓ Algorithm appears to follow O(log²(n)) complexity\n");
    } else if (correlation_log > correlation_log2 + 0.05) {
        printf("✓ Algorithm appears to follow O(log(n)) complexity\n");
    } else {
        printf("⚠ Similar correlations - complexity may be between log(n) and log²(n)\n");
    }
    
    // Check if ratios are roughly constant
    double first_ratio_log = avg_times[0] / log_sizes[0];
    double last_ratio_log = avg_times[num_sizes-1] / log_sizes[num_sizes-1];
    double ratio_growth_log = last_ratio_log / first_ratio_log;
    
    double first_ratio_log2 = avg_times[0] / log2_sizes[0];
    double last_ratio_log2 = avg_times[num_sizes-1] / log2_sizes[num_sizes-1];
    double ratio_growth_log2 = last_ratio_log2 / first_ratio_log2;
    
    printf("\nRATIO CONSISTENCY:\n");
    printf("Time/log(n) ratio growth:  %.2fx\n", ratio_growth_log);
    printf("Time/log²(n) ratio growth: %.2fx\n", ratio_growth_log2);
    
    printf("\nFor log(n): ");
    if (ratio_growth_log < 2.0)
        printf("✓ GOOD: Ratio stays relatively constant\n");
    else if (ratio_growth_log < 5.0)
        printf("⚠ MODERATE: Some growth beyond log(n)\n");
    else
        printf("✗ POOR: Significant growth beyond log(n)\n");
    
    printf("For log²(n): ");
    if (ratio_growth_log2 < 2.0)
        printf("✓ GOOD: Ratio stays relatively constant\n");
    else if (ratio_growth_log2 < 5.0)
        printf("⚠ MODERATE: Some growth beyond log²(n)\n");
    else
        printf("✗ POOR: Significant growth beyond log²(n)\n");
    
    free(avg_times);
    free(log_sizes);
    free(log2_sizes);
    return 0;
}

// Alternative: Test insertion time vs number of existing elements
void test_single_structure_growth()
{
    const int N = 100000;
    const int C = 8;
    const double TAU_0 = 0.5;
    const double TAU_D = 0.75;
    
    lla *test_lla = create_lla(N, C, TAU_0, TAU_D);
    
    printf("\n=== SINGLE STRUCTURE GROWTH TEST ===\n");
    printf("Measuring each insertion as structure grows...\n");
    printf("Size\tTime (μs)\tlog₂(n)\tlog²₂(n)\tTime/log(n)\tTime/log²(n)\n");
    printf("----\t---------\t-------\t--------\t-----------\t------------\n");
    
    // Measure every 1000th insertion
    for (int i = 1; i <= 50000; i++)
    {
        double start_time = get_time_us();
        insert(test_lla, rand() % (i * 10));
        double end_time = get_time_us();
        
        if (i % 1000 == 0)
        {
            double time_taken = end_time - start_time;
            double log_i = log2((double)i);
            double log2_i = log_i * log_i;
            printf("%d\t%.2f\t\t%.2f\t%.2f\t\t%.2f\t\t%.2f\n", 
                   i, time_taken, log_i, log2_i, time_taken/log_i, time_taken/log2_i);
        }
    }
    
    cleanup_lla(&test_lla);
}
