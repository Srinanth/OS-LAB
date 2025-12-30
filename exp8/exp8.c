#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int *numbers;
int count;

double mean_val;
double median_val;
double std_dev_val;

// Thread 1: Calculates the Mean
void *calculate_mean(void *param) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    
    mean_val = sum / count;
    pthread_exit(0);
}

// Helper function for sorting (needed for Median)
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Thread 2: Calculates the Median
void *calculate_median(void *param) {
    // Create a local copy to sort so we don't mess up the order for other threads
    // (Optional: depending on requirements, sorting the global array is risky if other threads read it simultaneously)
    int *sorted_nums = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) {
        sorted_nums[i] = numbers[i];
    }
    
    // Sort the array
    qsort(sorted_nums, count, sizeof(int), compare);

    if (count % 2 == 1) {
        // Odd number of elements
        median_val = sorted_nums[count / 2];
    } else {
        // Even number of elements: average the two middle ones
        int mid1 = sorted_nums[(count / 2) - 1];
        int mid2 = sorted_nums[count / 2];
        median_val = (mid1 + mid2) / 2.0;
    }
    
    free(sorted_nums);
    pthread_exit(0);
}

// Thread 3: Calculates the Standard Deviation
void *calculate_std_dev(void *param) {
    double sum = 0.0;
    double local_mean = 0.0;

    // 1. Calculate local mean first (to avoid waiting for the Mean thread)
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    local_mean = sum / count;

    // 2. Calculate sum of squared differences
    double sum_sq_diff = 0.0;
    for (int i = 0; i < count; i++) {
        sum_sq_diff += pow(numbers[i] - local_mean, 2);
    }

    // 3. Calculate Standard Deviation (Population formula)
    std_dev_val = sqrt(sum_sq_diff / count);
    
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <integer list>\n", argv[0]);
        return -1;
    }

    // Initialize global data
    count = argc - 1;
    numbers = malloc(count * sizeof(int));

    // Parse command line arguments into the integer array
    for (int i = 0; i < count; i++) {
        numbers[i] = atoi(argv[i + 1]);
    }

    // Identifiers for the threads
    pthread_t tid_mean;
    pthread_t tid_median;
    pthread_t tid_std_dev;

    // Create the threads
    // We pass NULL for attributes (default) and NULL for params (using globals)
    pthread_create(&tid_mean, NULL, calculate_mean, NULL);
    pthread_create(&tid_median, NULL, calculate_median, NULL);
    pthread_create(&tid_std_dev, NULL, calculate_std_dev, NULL);

    // Wait for the threads to exit
    pthread_join(tid_mean, NULL);
    pthread_join(tid_median, NULL);
    pthread_join(tid_std_dev, NULL);

    printf("The average value is %.2f\n", mean_val);
    printf("The median value is %.2f\n", median_val);
    printf("The standard deviation value is %.2f\n", std_dev_val);

    free(numbers);

    return 0;
}