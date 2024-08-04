#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initialize_vectors(int *A, int *B, int n) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int n = 100; // Tamanho dos vetores

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *A = NULL;
    int *B = NULL;
    int *C = NULL;

    if (rank == 0) {
        A = (int *)malloc(n * sizeof(int));
        B = (int *)malloc(n * sizeof(int));
        C = (int *)malloc(n * sizeof(int));
        srand(time(NULL));
        initialize_vectors(A, B, n);
    }

    int local_n = n / size;
    int *local_A = (int *)malloc(local_n * sizeof(int));
    int *local_B = (int *)malloc(local_n * sizeof(int));
    int *local_C = (int *)malloc(local_n * sizeof(int));

    MPI_Scatter(A, local_n, MPI_INT, local_A, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, local_n, MPI_INT, local_B, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++) {
        local_C[i] = local_A[i] + local_B[i];
    }

    MPI_Gather(local_C, local_n, MPI_INT, C, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Vetor C:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", C[i]);
        }
        printf("\n");

        free(A);
        free(B);
        free(C);
    }

    free(local_A);
    free(local_B);
    free(local_C);

    MPI_Finalize();
    return 0;
}
