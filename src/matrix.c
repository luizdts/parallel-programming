#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void matrix_mult(int n1, int n2, int n3, double **A, double **B, double **C)
{
    int i, j, k;
    double sum = 0.0;

    #pragma omp parallel \
        private(i, j, k) \
        shared(A, B, C) \
        firstprivate(sum) \
        num_threads(2)
    {
        #pragma omp for
        for (i = 0; i < n1; i++)
        {
            for (j = 0; j < n3; j++)
            {
                sum = 0.0;
                for (k = 0; k < n2; k++)
                {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
    }
}