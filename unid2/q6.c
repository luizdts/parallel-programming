/*
 * Compile:
 *    gcc -g -Wall -o q6 q6.c
 * Run:
 *    mpiexec -n <comm_sz> ./q6 <order of vector>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void Generate_order(int argc, char *argv[], int *n_p, int *local_n_p);
void Generate_vector(double local_vect[], int n, int local_n);
void Read_vector(char prompt[], double local_vect[], int n, int local_n);
void Print_vector(char title[], double local_vect[], int n, int local_n);
void prefix_sums(double local_vect[], double local_prefix_sums[], int n, int local_n);

int my_rank, comm_sz;
MPI_Comm comm;

int main(int argc, char *argv[])
{
    double *local_vect, *local_prefix_sums;
    int n, local_n;

    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    Generate_order(argc, argv, &n, &local_n);
    local_vect = malloc(local_n * sizeof(double));
    local_prefix_sums = malloc(local_n * sizeof(double));

    Generate_vector(local_vect, n, local_n);

    Print_vector("Vetor inicial para prefix sums: ", local_vect, n, local_n);
    prefix_sums(local_vect, local_prefix_sums, n, local_n);
    Print_vector("Resultado prefix sums: ", local_prefix_sums, n, local_n);

    free(local_vect);
    free(local_prefix_sums);

    MPI_Finalize();
    return 0;
}

/*-------------------------------------------------------------------*/
void Generate_order(int argc, char *argv[], int *n_p, int *local_n_p)
{
    if (my_rank == 0)
    {
        if (argc != 2)
            *n_p = 0;
        else
            *n_p = strtol(argv[1], NULL, 10);
    }

    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);

    *local_n_p = *n_p / comm_sz;
}

/*-------------------------------------------------------------------*/
void Generate_vector(double local_vect[], int n, int local_n)
{
    int i;
    double *tmp = NULL;

    if (my_rank == 0)
    {
        tmp = malloc(n * sizeof(double));
        srandom(time(NULL));
        for (i = 0; i < n; i++)
            tmp[i] = random() / ((double)RAND_MAX);
        MPI_Scatter(tmp, local_n, MPI_DOUBLE, local_vect, local_n, MPI_DOUBLE, 0,
                    comm);
        free(tmp);
    }
    else
    {
        MPI_Scatter(tmp, local_n, MPI_DOUBLE, local_vect, local_n, MPI_DOUBLE, 0,
                    comm);
    }
}

/*-------------------------------------------------------------------*/
void Read_vector(char prompt[], double local_vect[], int n, int local_n)
{
    int i;
    double *tmp = NULL;

    if (my_rank == 0)
    {
        tmp = malloc(n * sizeof(double));
        printf("%s\n", prompt);
        for (i = 0; i < n; i++)
            scanf("%lf", &tmp[i]);
        MPI_Scatter(tmp, local_n, MPI_DOUBLE, local_vect, local_n, MPI_DOUBLE, 0,
                    comm);
        free(tmp);
    }
    else
    {
        MPI_Scatter(tmp, local_n, MPI_DOUBLE, local_vect, local_n, MPI_DOUBLE, 0,
                    comm);
    }
}

/*-------------------------------------------------------------------*/
void Print_vector(char title[], double local_vect[], int n, int local_n)
{
    int i;
    double *tmp = NULL;

    if (my_rank == 0)
    {
        tmp = malloc(n * sizeof(double));
        MPI_Gather(local_vect, local_n, MPI_DOUBLE, tmp, local_n, MPI_DOUBLE, 0,
                   comm);
        printf("%s\n   ", title);
        for (i = 0; i < n; i++)
            printf("%.2f ", tmp[i]);
        printf("\n");
        free(tmp);
    }
    else
    {
        MPI_Gather(local_vect, local_n, MPI_DOUBLE, tmp, local_n, MPI_DOUBLE, 0,
                   comm);
    }
}

/*-------------------------------------------------------------------*/
void prefix_sums(double local_vect[], double local_prefix_sums[], int n,
                 int local_n)
{
    int local_i;
    double my_sum, pred_sum;

    local_prefix_sums[0] = local_vect[0];
    for (local_i = 1; local_i < local_n; local_i++)
        local_prefix_sums[local_i] = local_prefix_sums[local_i - 1] + local_vect[local_i];

    my_sum = local_prefix_sums[local_n - 1];
    MPI_Scan(&my_sum, &pred_sum, 1, MPI_DOUBLE, MPI_SUM, comm);

    pred_sum -= my_sum;
    for (local_i = 0; local_i < local_n; local_i++)
        local_prefix_sums[local_i] += pred_sum;
}