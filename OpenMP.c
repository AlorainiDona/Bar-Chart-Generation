#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void generateBarChart(int* data, int size) {
    // Find the maximum value in the data array
    int max = data[0];
    #pragma omp parallel for reduction(max:max)
    for (int i = 1; i < size; i++) {
        if (data[i] > max) {
            max = data[i];
        }
    }

// Create and initialize an array to count occurrences of each value
int* counts = (int*)calloc(max, sizeof(int));

#pragma omp parallel for schedule(dynamic)
for (int i = 0; i < size; i++) {
    #pragma omp atomic
    counts[data[i] - 1]++;
}

    // Print the bar chart
    for (int i = 0; i < max; i++) {
        printf("%d: ", i + 1);
        for (int j = 0; j < counts[i]; j++) {
            printf("*");
        }
        printf("\n");
    }

    // Free the dynamically allocated memory
    free(counts);
}

int main() {
    int size, num_threads;
    double start_time, end_time;

    printf("Enter the dataset size: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("Invalid size. Please enter a positive integer.\n");
        return 1;
    }

    int data[size];

    printf("Enter the number of threads for parallelization: ");
    scanf("%d", &num_threads);

    if (num_threads <= 0) {
        printf("Invalid number of threads. Please enter a positive integer.\n");
        return 1;
    }

    // Seed the random number generator
    srand(time(NULL));
    
    omp_set_num_threads(num_threads);
    
    for (int i = 0; i < size; i++) {
        data[i] = (rand() % size) + 1; // Generate random values in the range [1, size]
    }
    
    start_time = omp_get_wtime();
    printf("--- Bar chart ---\n");
    generateBarChart(data, size);
    end_time = omp_get_wtime();
    
    double execution_time = end_time - start_time;
    printf("Execution time: %f seconds\n", execution_time);

    return 0;
}

/* #include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void generateBarChart(int* data, int size, int num_threads) {
    // Find the maximum value in the data array
    int max = data[0];
    #pragma omp parallel for reduction(max:max) num_threads(num_threads)
    for (int i = 1; i < size; i++) {
        if (data[i] > max) {
            max = data[i];
        }
    }

    // Create an array to count occurrences of each value
    int counts[max];
    for (int i = 0; i < max; i++) {
        counts[i] = 0;
    }

    // Parallelize the counting loop
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < size; i++) {
        #pragma omp atomic
        counts[data[i] - 1]++;
    }

    // Print the bar chart
    for (int i = 0; i < max; i++) {
        printf("%d: ", i + 1);
        for (int j = 0; j < counts[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
}

int main() {
    int size, num_threads;
    double start_time, end_time;

    printf("Enter the dataset size: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("Invalid size. Please enter a positive integer.\n");
        return 1;
    }

    int data[size];

    printf("Enter the dataset values (separated by spaces): ");
    for (int i = 0; i < size; i++) {
        scanf("%d", &data[i]);
    }

    printf("Enter the number of threads for parallelization: ");
    scanf("%d", &num_threads);

    if (num_threads <= 0) {
        printf("Invalid number of threads. Please enter a positive integer.\n");
        return 1;
    }

    start_time = omp_get_wtime();
    printf("--- Bar chart ---\n");
    generateBarChart(data, size, num_threads);
    end_time = omp_get_wtime();
    double execution_time = end_time - start_time;
    printf("Execution time: %f seconds\n", execution_time);
    return 0;
}

*/


