#include <stdio.h>
#include <mpi.h>

const int PACK_BUF_SIZE = 100;

void Get_input(int my_rank, double *a_p, double *b_p, int *n_p)
{
    int pack_buf = PACK_BUF_SIZE;
    int position = 0;
    int dest;

    if (my_rank == 0)
    {
        printf("Enter a, b, and n\n");

        scanf("%lf %lf %d", a_p, b_p, n_p);
        MPI_Pack(a_p, 1, MPI_DOUBLE, pack_buf, PACK_BUF_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(b_p, 1, MPI_DOUBLE, pack_buf, PACK_BUF_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(n_p, 1, MPI_INT, pack_buf, PACK_BUF_SIZE, &position, MPI_COMM_WORLD);
    }

    MPI_BCast(pack_buf, PACK_BUF_SIZE, MPI_PACKED, 0, MPI_COMM_WORLD);

    if (my_rank > 0)
    {

        MPI_Unpack(pack_buf, PACK_BUF_SIZE, &position, a_p, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(pack_buf, PACK_BUF_SIZE, &position, b_p, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(pack_buf, PACK_BUF_SIZE, &position, n_p, 1, MPI_INT, MPI_COMM_WORLD);
    }
}
