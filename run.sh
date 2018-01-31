#!/bin/sh

cd /home/lciuffre/P1.6_project/examples
module load openmpi

echo "N=108"
START=$(date +%s.%N)
../ljmd-serial.x < argon_108.inp
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF
export OMP_NUM_THREADS=20
START=$(date +%s.%N)
../ljmd-omp.x < argon_108.inp
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF
START=$(date +%s.%N)
mpirun -np 2 ../ljmd_MPI_din.x < argon_108.inp
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF
export OMP_NUM_THREADS=10
START=$(date +%s.%N)
mpirun -np 2 ../ljmd-hybrid.x < argon_108.inp
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF

echo "N=2196"
START=$(date +%s.%N)
../ljmd-serial.x < argon_2196.inp
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF
export OMP_NUM_THREADS=20
START=$(date +%s.%N)
../ljmd-omp.x < argon_2196.inp
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF
START=$(date +%s.%N)
mpirun -np 2 ../ljmd_MPI_din.x < argon_2196.inp
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF
export OMP_NUM_THREADS=10
START=$(date +%s.%N)
mpirun -np 2 ../ljmd-hybrid.x < argon_2196.inp
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF
