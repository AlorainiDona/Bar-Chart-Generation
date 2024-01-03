#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateBarChart(int* data, int size) {
    // Find the maximum value in the data array
    int max = data[0];
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

    for (int i = 0; i < size; i++) {
        counts[data[i] - 1]++;
    }

    for (int i = 0; i < max; i++) {
        printf("%d: ", i + 1);
        for (int j = 0; j < counts[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
}

int main() {
    int size;
    clock_t start_time, end_time;
    
    printf("Enter the dataset size: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("Invalid size. Please enter a positive integer.\n");
        return 1;
    }

    int data[size];

    // Seed the random number generator
    srand(time(NULL));
    

    for (int i = 0; i < size; i++) {
        data[i] = (rand() % size) + 1; // Generate random values in the range [1, size]
    }

    start_time = clock();
    
    printf("--- Bar chart ---\n");
    generateBarChart(data, size);

    end_time = clock();

    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", execution_time);
    return 0;
}


/*#include <stdio.h>

void generateBarChart(int* data, int size) {
    // Find the maximum value in the data array
    int max = data[0];
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

    for (int i = 0; i < size; i++) {
        counts[data[i] - 1]++;
    }

    for (int i = 0; i < max; i++) {
        printf("%d: ", i + 1);
        for (int j = 0; j < counts[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
}

int main() {
    int size;
    clock_t start_time, end_time;

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
    start_time = clock();

    printf("--- Bar chart ---\n");
    generateBarChart(data, size);
    end_time = clock();

    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", execution_time);

    return 0;
}
*/
