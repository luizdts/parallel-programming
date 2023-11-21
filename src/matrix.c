#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define thread_count 4

void matrix_mult(int n1, int n2, int n3, double **A, double **B, double **C)
{

    int i, j, k;
    double sum = 0.0;
#pragma omp parallel private(i, j, k, sum) shared(A, B, C) num_threads(thread_count)
    for (i = 0; i < n1; i++)
    {
        int tid = omp_get_thread_num();
        printf("%d", tid);
        for (j = 0; j < n3; j++)
        {
            for (k = 0; k < n2; k++)
            {
                sum = sum + A[i][k] * B[k][j];
            }
            C[i][j] = sum;
            sum = 0.0;
        }
    }
}