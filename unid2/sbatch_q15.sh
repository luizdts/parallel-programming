#!/bin/bash
#SBATCH --job-name=q15_npad
#SBATCH --time=0-00:30:00
#SBATCH --ntasks-per-node=32
#SBATCH --partition=test
#SBATCH --exclusive
#SBATCH --hint=compute_bound
#SBATCH --nodes=1

#número de processos
n_processes=(1 2 4 8 16 32)

arg_values=(200000000 400000000 500000000 1000000000 2000000000)

for n_procs in "${n_processes[@]}"; do
    for arg_value in "${arg_values[@]}"; do
        echo "Executando mpirun -np $n_procs ./q15 g $arg_value"
        result=$(srun --ntasks $n_procs ./q15 g $arg_value)
        echo "Resultado para $n_procs processos e tamanho de entrada $arg_value:"
        echo "$result"
        echo "---------------------------------------------"
    done
done