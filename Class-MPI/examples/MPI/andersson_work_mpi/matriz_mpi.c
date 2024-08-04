#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 1000

void generate_matrix(int *matrix, int size) {
    for (int i = 0; i < size; i++) {
        matrix[i] = (rand() % 201) - 100;
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *matrix = NULL;
    int *local_matrix = (int *)malloc(N * N / size * sizeof(int));

    if (rank == 0) {
        matrix = (int *)malloc(N * N * sizeof(int));
        srand(time(NULL));
        generate_matrix(matrix, N * N);
    }

    MPI_Scatter(matrix, N * N / size, MPI_INT, local_matrix, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    int local_negative_count = 0, local_zero_count = 0;
    long long local_sum = 0;

    int local_max = local_matrix[0];
    int local_min = local_matrix[0];

    for (int i = 0; i < N * N / size; i++) {
        if (local_matrix[i] < 0) local_negative_count++;
        if (local_matrix[i] == 0) local_zero_count++;
        if (local_matrix[i] > local_max) local_max = local_matrix[i];
        if (local_matrix[i] < local_min) local_min = local_matrix[i];
        local_sum += local_matrix[i];
    }

    int global_negative_count, global_zero_count;
    int global_max, global_min;
    long long global_sum;
    MPI_Reduce(&local_negative_count, &global_negative_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_zero_count, &global_zero_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double mean = (double)global_sum / (N * N);
        double variance_sum = 0.0;

        for (int i = 0; i < N * N; i++) {
            variance_sum += (matrix[i] - mean) * (matrix[i] - mean);
        }

        double stddev = sqrt(variance_sum / (N * N));

        printf("Número de elementos negativos: %d\n", global_negative_count);
        printf("Maior valor: %d\n", global_max);
        printf("Menor valor: %d\n", global_min);
        printf("Média: %f\n", mean);
        printf("Desvio Padrão: %f\n", stddev);
        printf("Número de zeros: %d\n", global_zero_count);
        
        if (global_zero_count > (N * N) / 2)
            printf("A matriz é esparsa.\n");
        else
            printf("A matriz não é esparsa.\n");
    
        free(matrix);
    }

    free(local_matrix);
    MPI_Finalize();
    return 0;
}
