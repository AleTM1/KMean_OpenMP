#!/bin/bash
#SBATCH --job-name=nomejob
#SBATCH --mail-type=ALL
#SBATCH --mail-user=alessandro.maggio5@studio.unibo.it
#SBATCH --time=01:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --output=execution_log
#SBATCH --gres=gpu:1


make clean all

OMP_NUM_THREADS=8 ./project_kmean

