#!/bin/bash
#
## SLURM submission script for OpenMP job jacobi
## change cpus-per-task to change number of threads
#SBATCH --job-name=pagerank_omp
#SBATCH --output=outputs/pagerank_omp.%J.out
#SBATCH --error=outputs/pagerank_omp.%J.err
#
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1   ## controls number of threads
#SBATCH --time=5:00
#SBATCH --partition=class

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
## change n to modify number gridpoints in the rod
## change s to modify number of outer jacobi sweeps or iterations
./bin/pgrk1 1600
