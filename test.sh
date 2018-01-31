#!/bin/bash

#PBS -l nodes=1:ppn=20
#PBS -l walltime=00:10:00
#PBS -N benchmark_ljmd

cd /home/lciuffre/P1.6_project
module load openmpi
./run.sh > timings.out
