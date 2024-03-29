#!/bin/bash
#SBATCH --job-name=<q12>
#SBATCH --time=2-0:0
#SBATCH --cpus-per-task=1
#SBATCH --partition=cluster
#SBATCH --exclusive

mpiexec -n 8 ./q12
