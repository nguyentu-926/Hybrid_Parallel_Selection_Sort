// file: selection_sort_serial.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void selection_sort(int *arr, int n, int ascending) {
    for (int i = 0; i < n - 1; i++) {
        int idx = i;
        for (int j = i + 1; j < n; j++) {
            if ((ascending && arr[j] < arr[idx]) || (!ascending && arr[j] > arr[idx])) {
                idx = j;
            }
        }
        int temp = arr[idx];
        arr[idx] = arr[i];
        arr[i] = temp;
    }
}

int compare_asc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int compare_desc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

int main(int argc, char** argv) {
    int rank, size, N = 10000;
    int *data = NULL, *local_data;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = N / size;

    if (rank == 0) {
        data = malloc(N * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < N; i++) {
            data[i] = rand() % 100000;
        }
        start = MPI_Wtime();
    }

    local_data = malloc(local_n * sizeof(int));
    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    selection_sort(local_data, local_n, 1); // ascending

    int *sorted_data = NULL;
    if (rank == 0) {
        sorted_data = malloc(N * sizeof(int));
    }

    MPI_Gather(local_data, local_n, MPI_INT, sorted_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        qsort(sorted_data, N, sizeof(int), compare_asc);
        end = MPI_Wtime();
        printf("Sorted ascending in %lf seconds (MPI with %d processes)\n", end - start, size);

        qsort(sorted_data, N, sizeof(int), compare_desc);
        printf("Sorted descending in %lf seconds (after ascending)\n", MPI_Wtime() - end);

        free(data);
        free(sorted_data);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}
