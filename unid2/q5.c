/*
 * Compile:  mpicc -g -Wall -o q4 q4.c
 * Run:      mpiexec -n 1 ./q4
 *
 * Input:    The order of the vectors, n, and the vectors x and y
 * Output:   The sum vector z = x+y mpi_vector_add.c

 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Read_n(int *n_p, int *local_n_p, int my_rank, int comm_sz,
            MPI_Comm comm);
void Allocate_vectors(double **local_x_pp, double **local_y_pp,
                      double **local_z_pp, int local_n, MPI_Comm comm);
void Read_vector(double local_a[], int local_n, int n, char vec_name[],
                 int my_rank, MPI_Comm comm);
void Print_vector(double local_b[], int local_n, int n, char title[],
                  int my_rank, MPI_Comm comm);

void Read_escalar(double *scalar, int my_rank, MPI_Comm comm);

void mult_vetor_escalar(double local_x[], double escalar, double local_z[], int local_n);
double produto_escalar(double local_x[], double local_y[], MPI_Comm comm, int local_n, double *prod_scalar);

/*-------------------------------------------------------------------*/
int main(void)
{
    int n, local_n;
    int comm_sz, my_rank;
    double *local_x, *local_y, *local_z;
    double escalar, prod_escalar;
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    Read_n(&n, &local_n, my_rank, comm_sz, comm);
    Allocate_vectors(&local_x, &local_y, &local_z, local_n, comm);

    Read_escalar(&escalar, my_rank, comm);

    Read_vector(local_x, local_n, n, "1", my_rank, comm);
    Print_vector(local_x, local_n, n, "Vetor 1 is", my_rank, comm);

    Read_vector(local_y, local_n, n, "2", my_rank, comm);
    Print_vector(local_y, local_n, n, "Vetor 2 is", my_rank, comm);

    produto_escalar(local_x, local_y, comm, local_n, &prod_escalar);
    if (my_rank == 0)
    {
        printf("Resultado do produto escalar: \n%lf\n", prod_escalar);
    }

    if (my_rank == 0)
    {
        printf("Multiplicação do vetor 1 pelo escalar: \n%lf\n", escalar);
    }

    mult_vetor_escalar(local_x, escalar, local_z, local_n);
    Print_vector(local_z, local_n, n, "", my_rank, comm);

    if (my_rank == 0)
    {
        printf("Multiplicação do vetor 2 pelo escalar: \n%lf\n", escalar);
    }

    mult_vetor_escalar(local_x, escalar, local_z, local_n);
    Print_vector(local_z, local_n, n, "", my_rank, comm);

    free(local_x);
    free(local_y);
    free(local_z);

    MPI_Finalize();

    return 0;
}

/*-------------------------------------------------------------------*/
void Read_n(
    int *n_p,
    int *local_n_p,
    int my_rank,
    int comm_sz,
    MPI_Comm comm)
{

    if (my_rank == 0)
    {
        printf("ordem dos vetores 1 e 2: \n");
        scanf("%d", n_p);
    }
    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);

    *local_n_p = *n_p / comm_sz;
}

/*-------------------------------------------------------------------*/
void Allocate_vectors(
    double **local_x_pp,
    double **local_y_pp,
    double **local_z_pp,
    int local_n,
    MPI_Comm comm)
{
    *local_x_pp = malloc(local_n * sizeof(double));
    *local_y_pp = malloc(local_n * sizeof(double));
    *local_z_pp = malloc(local_n * sizeof(double));
}

/*-------------------------------------------------------------------*/
void Read_vector(
    double local_a[],
    int local_n,
    int n,
    char vec_name[],
    int my_rank,
    MPI_Comm comm)
{

    double *a = NULL;
    int i;

    if (my_rank == 0)
    {
        a = malloc(n * sizeof(double));
        printf("valores do vetor %s\n", vec_name);
        for (i = 0; i < n; i++)
            scanf("%lf", &a[i]);
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0,
                    comm);
        free(a);
    }
    else
    {
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0,
                    comm);
    }
}

/*-------------------------------------------------------------------*/
void Print_vector(
    double local_b[],
    int local_n,
    int n,
    char title[],
    int my_rank,
    MPI_Comm comm)
{

    double *b = NULL;
    int i;

    if (my_rank == 0)
    {
        b = malloc(n * sizeof(double));
        MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
        printf("%s\n", title);
        for (i = 0; i < n; i++)
        {
            printf("%f ", b[i]);
        }
        printf("\n");
        free(b);
    }
    else
    {
        MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
    }
}
/*-------------------------------------------------------------------*/
void Read_escalar(
    double *escalar,
    int my_rank,
    MPI_Comm comm)
{
    if (my_rank == 0)
    {
        printf("defina o escalar: \n");
        scanf("%lf", escalar);
    }
    MPI_Bcast(escalar, 1, MPI_DOUBLE, 0, comm);
}

/*-------------------------------------------------------------------*/

double produto_escalar(
    double local_x[],
    double local_y[],
    MPI_Comm comm,
    int local_n,
    double *prod_scalar)
{
    double prod_escalar, local_prod_scalar = 0;
    int local_i;

    for (local_i = 0; local_i < local_n; local_i++)
    {
        local_prod_scalar += local_x[local_i] * local_y[local_i];
    }

    MPI_Reduce(&local_prod_scalar, prod_scalar, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

    return prod_escalar;
}

/*-------------------------------------------------------------------*/
void mult_vetor_escalar(
    double local_x[],
    double escalar,
    double local_z[],
    int local_n)
{
    int local_i;

    for (local_i = 0; local_i < local_n; local_i++)
    {
        local_z[local_i] = local_x[local_i] * escalar;
    }
}