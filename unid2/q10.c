/* File:
 *    ex3.19_mpi_type_indexed.c
 *
 * Description:
 *    Send the upper triangular part of a square matrix from
 *    process 0 to process 1.  Use MPI_Type_indexed to create
 *    a derived datatype that can be used in the communication
 *
 * Compile:
 *    mpicc -g -Wall -o ex3.19_mpi_type_indexed ex3.19_mpi_type_indexed.c
 * Usage:
 *    mpiexec -n 2 ./ex3.19_mpi_type_indexed
 *
 * Input:
 *    Order of matrix and matrix
 *
 * Output:
 *    Upper triangular part of the input matrix
 *
 * Notes:
 * 1.  The program must be run with at least two processes.
 * 2.  If more than two processes are used, processes with
 *     rank >= 2 do nothing.
 *
 * IPP:  Exercise 3.19
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int my_rank, comm_sz;
MPI_Comm comm;

void ordem_matriz(int *n_p);
void read_matriz(double matrix[], int n);
void build_tipo_indexado(int n, MPI_Datatype *indexed_p);
void imprime_matriz(char title[], double matrix[], int n);

int main(void)
{
    int n;
    double *matrix;
    MPI_Datatype indexed;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    ordem_matriz(&n);
    matrix = malloc(n * n * sizeof(double));

    build_tipo_indexado(n, &indexed);
    if (my_rank == 0)
    {
        read_matriz(matrix, n);

        MPI_Send(matrix, 1, indexed, 1, 0, comm);
    }
    else if (my_rank == 1)
    {
        memset(matrix, 0, n * n * sizeof(double));
        MPI_Recv(matrix, 1, indexed, 0, 0, comm, MPI_STATUS_IGNORE);
        imprime_matriz(matrix, n);
    }

    free(matrix);
    MPI_Type_free(&indexed);
    MPI_Finalize();
    return 0;
}

/*-------------------------------------------------------------------*/
void ordem_matriz(int *n_p)
{

    if (my_rank == 0)
    {
        printf("Enter the order of the matrix\n");
        scanf("%d", n_p);
    }

    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
}

/*-------------------------------------------------------------------*/
void build_tipo_indexado(int n, MPI_Datatype *indexed)
{
    int i;
    int *array_of_block_lens;
    int *array_of_disps;

    array_of_block_lens = malloc(n * sizeof(int));
    array_of_disps = malloc(n * sizeof(int));

    for (i = 0; i < n; i++)
    {
        array_of_block_lens[i] = n - i;
        array_of_disps[i] = i * (n + 1);
    }

    MPI_Type_indexed(n, array_of_block_lens, array_of_disps, MPI_DOUBLE, indexed_p);
    MPI_Type_commit(indexed);

    free(array_of_block_lens);
    free(array_of_disps);
}

/*-------------------------------------------------------------------*/
void read_matriz(double matrix[], int n)
{
    int i, j;

    printf("Insira a matrix\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%lf", &matrix[i * n + j]);
}

/*-------------------------------------------------------------------*/
void imprime_matriz(double matrix[], int n)
{
    int i, j;

    printf("Proc %d > %s\n", my_rank, title);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            printf("%.2f ", matrix[i * n + j]);
        printf("\n");
    }
    printf("\n");
}