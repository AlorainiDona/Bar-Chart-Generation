#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <mpi.h>
#include <time.h>

void generateBarChart(int rank, int size, int* data, int data_size) {
    int local_size = data_size / size;
    int remainder = data_size % size;

    int start = rank * local_size + (rank < remainder ? rank : remainder);
    int end = start + local_size + (rank < remainder ? 1 : 0);

    int max = 0;
    for (int i = start; i < end; i++) {
        if (data[i] > max) {
            max = data[i];
        }
    }

    MPI_Allreduce(MPI_IN_PLACE, &max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    int* counts = (int*)calloc(max, sizeof(int));

    for (int i = start; i < end; i++) {
        counts[data[i] - 1]++;
    }

    int* all_counts = NULL;
    int* recv_counts = NULL;
    int* displacements = NULL;

    if (rank == 0) {
        all_counts = (int*)calloc(max * size, sizeof(int));
        recv_counts = (int*)malloc(size * sizeof(int));
        displacements = (int*)malloc(size * sizeof(int));
    }

    MPI_Gather(&max, 1, MPI_INT, recv_counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        displacements[0] = 0;
        for (int i = 1; i < size; i++) {
            displacements[i] = displacements[i - 1] + recv_counts[i - 1];
        }
    }

    MPI_Gatherv(counts, max, MPI_INT, all_counts, recv_counts, displacements, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < max; i++) {
            printf("%d: ", i + 1);
            for (int j = 0; j < size; j++) {
                if (i < recv_counts[j]) {
                    for (int k = 0; k < all_counts[displacements[j] + i]; k++) {
                        printf("*");
                    }
                }
            }
            printf("\n");
        }
        free(all_counts);
        free(recv_counts);
        free(displacements);
    }

    free(counts);
}

int main(int argc, char *argv[]) {
    int rank, size, data_size;
    int *data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time, end_time, total_time = 0.0;

    if (rank == 0) {
        printf("Enter the dataset size: ");
        scanf("%d", &data_size);

        data = (int*)malloc(data_size * sizeof(int));
        printf("--- Bar chart ---");
        printf("\n");

        srand((unsigned int)time(NULL));

        for (int i = 0; i < data_size; i++) {
            data[i] = rand() % data_size + 1;
        }

        start_time = MPI_Wtime();
    }

    MPI_Bcast(&data_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        data = (int*)malloc(data_size * sizeof(int));
    }

    MPI_Bcast(data, data_size, MPI_INT, 0, MPI_COMM_WORLD);

    generateBarChart(rank, size, data, data_size);

    end_time = MPI_Wtime();

    double* all_times = NULL;
    if (rank == 0) {
        all_times = (double*)malloc(size * sizeof(double));
    }
    MPI_Gather(&end_time, 1, MPI_DOUBLE, all_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            total_time += all_times[i] - start_time;
        }
        printf("Total execution time across all processes: %.6f seconds\n", total_time);
        free(data);
        free(all_times);
    }

    MPI_Finalize();
    return 0;
}
