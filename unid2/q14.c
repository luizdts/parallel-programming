A// Questão 14 -- NPAD
// compile: mpicc q14.c -o q14

#include <stdio.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int my_rank, comm_sz;
    int ping = 0;
    int pong = 1;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    clock_t start_clock = clock();
    double start_MPI = MPI_Wtime();

    if (my_rank == 0)
    {
        MPI_Send(&ping, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Processo %d enviou: %d\n", my_rank, ping);
        MPI_Recv(&pong, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu: %d\n", my_rank, pong);
    }
    else
    {
        MPI_Recv(&ping, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu: %d\n", my_rank, ping);
        MPI_Send(&pong, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Processo %d enviou: %d\n", my_rank, pong);
    }

    clock_t finish_clock = clock();
    double finish_MPI = MPI_Wtime();

    double tempo_clock = (double)(finish_clock - start_clock) / CLOCKS_PER_SEC;
    double tempo_MPI = finish_MPI - start_MPI;

    if (my_rank == 0)
    {
        printf("Tempo de execução do clock: %f\n", tempo_clock);
        printf("Tempo de execução do MPI_Wtime: %f\n", tempo_MPI);
    }

    MPI_Finalize();
}
