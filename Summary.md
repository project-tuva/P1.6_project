*******************************************************************************
A SIMPLE LJ MANY-BODY SIMULATOR OPTIMIZATION AND PARALLELIZATION
*******************************************************************************
THE LJ MODEL FOR LIQUID ARGON
-simulate a cubic box of particles with a Lennard-Jones type pairwise additive
 interaction potential
 (https://en.wikipedia.org/wiki/Lennard-Jones_potential)
-Periodic boundary conditions to avoid surface effects
*******************************************************************************
NEWTON'S LAWS OF MOTION
We consider our particles to be classical objects
so Newton's laws of motion apply:
1. In absence of a force, a body rests or moves in a straight line
with constant velocity
2. A body experiencing a force F experiences an acceleration a
related to F by F = ma, where m is the mass of the body
(More force, more acceleration)
3. Whenever a first body exerts a force F on a second body,
the second body exerts a force −F on the first body
(Force is simmetric: opposite sign respect to the adjacent particle)
*******************************************************************************
VELOCITY VERLET ALGORITHM (DA CAPIRE!!!!!!!!)
-The velocity Verlet algorithm is used to propagate the positions of the atoms
 (https://en.wikipedia.org/wiki/Verlet_integration)
-We integrate the equation of motion to know the position
-The forces are defined by the initial position of the particles
******************************************************************************
WHAT DO WE NEED TO PROGRAM? (DA CAPIRE!!!!!!!!)
1. Read in parameters and initial status and compute what is missing
(e.g. accelerations)
We need 1 array to store velocity/position/forces
2. Integrate Equations of motion with Velocity Verlet
for a given number of steps
a) Propagate all velocities for half a step
b) Propagate all positions for a full step
c) Compute forces on all atoms to get accelerations
d) Propagate all velocities for half a step
e) Output intermediate results, if needed
******************************************************************************
INITIAL SERIAL CODE: VELOCITY VERLET (1)
Uses a struct in c: passed as a pointer to the subroutines
******************************************************************************
INITIAL CODE: FORCE CALCULATION (2)
-potential:gives us the energy
-cutoff distance (rcut): after which no more force
-the energy depends on the position
but now we're computing twice times the forces!
******************************************************************************
HOW WELL DOES IT WORK?
gcc ­o ljmd.x ljmd.c ­lm
Test input: 108 atoms, 10000 steps: 49s
25% of time calling pbc:
-periodic boundary condition
-very fast function but calling it has an overhead
******************************************************************************
COMPILER OPTIMIZATION
(1) Use of pbc() is convenient, but costs 25% => compiling with -O3
    should inline it (copy the body of the function into the code)
(2) Loops should be unrolled for superscalar CPUs => compiling with -O2 or -O3
    should do it for us
    Time now: 39s (1.3x faster) --> Only a bit faster
(3) Now try some more optimization options:
    -ffast-math -fexpensive-optimizations -msse3
    Time now: 10s (4.9x faster) --> Much better!
    (NB) It's faster but we've to check the result
(4) Compare to LAMMPS: 3.6s => need to do more
******************************************************************************
NOW MODIFY THE CODE
(1) Use physics! Newton's 3 rd law: Fij=-Fji (not look at every pair twice)
    Time now: 5.4s (9.0x faster) --> Another big improvement
(2) Avoid expensive math: pow(), sqrt(), division
=> 108 atoms: 4.0s (12.2x faster) --> still worth 
******************************************************************************
IMPROVEMENTS SO FAR
(1) Use the optimal compiler flags => ~5x faster (4.9)
but some of it: inlining, unrolling could be coded
(2) Use our knowledge of physics => ~2x faster ?????????
since we need to compute only half the data.
(3) Use our knowledge of computer hardware
=> 1.35x faster. (there could be more: SSE) ?????????
We are within 10% (4s vs. 3.6s) of LAMMPS
so we could be happy
BUT
when we try a bigger system: 2'916 atoms, 100 steps
Our code: 13.3s LAMMPS: 2.7s => Bad scaling with system size
******************************************************************************
MAKING IT SCALE WITH SYSTEM SIZE (DA CAPIRE!!!!!!!!)
-We compute all distances between pairs
-But for larger systems not all pairs contribute yet our effort is O(N^2)
-Avoid distant pairs, so
  -Divide system in cells of size >= cutoff.
  -Sort atoms into cells
  -Look only at 26 cells around central cell
  (26 = 3*3*3-1)
******************************************************************************
THE CELL-LIST VARIANT (DA CAPIRE!!!!!!!!)
(1) At startup, build a list of lists to store atom indices for atoms
that “belong” to a cell
(2) Compute a list of pairs between cells which contain atoms within cutoff.
Doesn't change!
(3) During Molecular Dynamics sort atoms into cells
(4) Then loop over list of “close” pairs of cells i and j
(5) For pair of cells loop over pairs of atoms in them
(6) Now we have linear scaling with system size at the cost of using
more memory and an O(N) sort
******************************************************************************
CELL LIST LOOP
-2'916 atom time: 3.4s (4x faster), LAMMPS 2.7s
-The cell dimesnion has to be at least the cutoff
******************************************************************************
SCALING WITH SYSTEM SIZE (size and time)
Cell list does not help (or hurt) much for small inputs,
but is a huge win for larger problems
=> Lesson: always pay attention to scaling

******************************************************************************
WHAT IF OPTIMIZATION IS NOT ENOUGH?
(1) Having linear scaling is nice, but twice the system size is still
twice the work => Parallelization
(2) Simple MPI parallelization first
 -MPI is “share nothing” (replicated or distributed data)
 -Run the same code path with the same data
but insert a few MPI calls
  -Broadcast positions from rank 0 to all before force()
  -Compute forces on different atoms for each rank
  -Collect (reduce) forces from all to rank 0 after force()
******************************************************************************x
REPLICATED DATA MPI VERSION
Easy to implement, but lots of communication
******************************************************************************
MPI PARALLEL
-EFFICIENCY
- EXECUTION TIMES
(DA CAPIRE!!!!!!!!)
******************************************************************************
OPENMP PARALLELIZATION
OpenMP is
-directive based => code (can) work without them
- can be added incrementally
- only works in shared memory => multi-core processors
- hides the calls to a threads library => less flexible, but less programming

Caution: write access to shared data can easily lead to race conditions
******************************************************************************
NAIVE OPENMP VERSION
-each thread will work on different values of "i"
-Race condition: “i” will be unique for each thread, but not “j”
=> multiple threads may write to the same location concurrently
so
“critical” directive: will let only one thread at a time execute this block
but it turn to be slower
******************************************************************************
OPENMP IMPROVEMENTS
(1) Use omp atomic to protect one instruction
=> faster, but requires hardware support
-- 108:---
1T: 6.3s,
2T: 5.0s,
4T: 4.4s,
8T: 4.2s
--- 2916:---
1T: 126s,
2T: 73s,
4T: 48s,
8T: 26s
=> some speedup, but noticable overhead
=> serial is faster than OpenMP with 1T

(2) Don't use Newton's 3 Law => no race condition
--- 108:---
1T: 6.5s,
2T: 3.7s,
4T: 2.3s,
8T: 2.1s
--- 2916:---
1T: 213s,
2T: 106s,
4T: 53s,
8T: 21s
=> better scaling, but we lose 2x serial speed
******************************************************************************
MPI-LIKE APPROACH WITH OPENMP
(1) Thread Id is like MPI rank
(2) sys->fx holds storage for one full fx array for each thread
    => race condition is eliminated.
(3) We need to write our own reduction:
 -Need to make certain, all threads are done with computing forces
 -Use threads to parallelize the reductions
******************************************************************************
MORE OPENMP TIMINGS
(1) The omp parallel region timings
--- 108:---
1T: 3.5s,
2T: 2.5s,
4T: 2.2s,
8T: 2.5s
--- 2916:---
1T: 103s,
2T: 53s,
4T: 19s,
8T: 10s
=> better speedup, 1T is about as fast as serial
=> scaling like no 3 rd law, but speed 2x as fast

(2) This approach also works with cell lists:
--- 108:---
1T: 4.3s,
2T: 3.1s,
4T: 2.4s,
8T: 2.9s
--- 2916:---
1T: 28s,
2T: 15s,
4T: 8.9s,
8T: 4.1s
=> 6.8x speedup with 8 threads.
62x faster than original code with 2916 atoms
******************************************************************************
HYBRID OPENMP/MPI VERSION
(1) With multi-core nodes, communication between MPI tasks becomes a problem
=> all communication has to us one link
=> reduced bandwidth, increased latency
(2) OpenMP and MPI parallelization are orthogonal and can be used at the same t
Caution: don't call MPI from threaded region
(3) Parallel region OpenMP version is very similar to MPI version,
so that would be easy to merge
******************************************************************************
HYBRID OPENMP/MPI KERNEL
(1) MPI tasks are like GPU thread blocks
(2) Need to reduce forces/energies first across th and then across all MPI tasks
******************************************************************************
HYBRID OPENMP/MPI TIMINGS
Replicated data MPI is simple to implement
but does not parallelize well for this kind of code
******************************************************************************
CONCLUSIONS
(1) Make sure that you exploit the physics of your problem well
=> Newton's 3 rd law gives a 2x speedup for free
(but interferes with threading!)
(2) Let the compiler help you (more readable code), but also make it easy
to the compiler
=> unrolling, inlining can be offloaded
(3) Understand the properties of your hardware and adjust your code to match it
(4) For a large number of threads use simpler code