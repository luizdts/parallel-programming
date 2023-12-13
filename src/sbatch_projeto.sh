#!/bin/bash
#SBATCH --job-name=svd
#SBATCH --time=0-00:30:00
#SBATCH --nodes=1

#SBATCH --ntasks-per-node=1
#SBATCH --partition=amd-512

#SBATCH --exclusive
#SBATCH --hint=compute_bound

pascalanalyzer -t aut -c 1,8,16,32,64,128 -o SVD.json ./svd