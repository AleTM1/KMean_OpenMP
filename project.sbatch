#!/bin/bash
#SBATCH --job-name=kmean_proj
#SBATCH --mail-type=ALL
#SBATCH --mail-user=alessandro.maggio5@studio.unibo.it
#SBATCH --time=01:00:00
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=4
#SBATCH --output=execution_log
#SBATCH --gres=gpu:1


make clean all

./project_kmean 'data10000_3.csv' 'kmedoids' 4 3 'kmeanpp'

