# INDIVIDUAL PART
## Introduction
This section describes the code optimization.
The main goal is to optimize the force computation, refactoring the code in
order to avoid costly
operations or redundant work. The improvements are documented with
profiling/benchmark data.
So two steps will be perfomed:
1. Mapping, through the visualization of the call-tree and code coverage
2. Profiling, analysing the code behaviour (hotspots, bottlenecks, resource
utilization) and its
efficiency (instruction cycles/cycles, L1/L2/RAM hits, branch misses);

## Starting code
After breaking down the single file ljmd.c into multiple files and updating
the make process accordingly so that dependencies are properly applied,
__gprof__ was used to produce the call-tree.

The call-tree:
* is a control flow graph that exhibits the calling relationship among
routines in a program;
* reports the cumulative number of calls.
* a node in the graph represents a routine while an edge represent a calling
relationship.

After compiling with ```make serial``` using the following flags:
```
CFLAGS=-Wall -std=c99 -pg -g -fno-pie -I$(HEADDIR)
LDLIBS=-lm -pg -fno-pie
```
and running the code for 108 atoms and  10'000 steps
* the execution time: 28 s (vs 49 seconds)
* the file  __gmon.out__ needed to gprof, was obtained.
```
%   cumulative   self              self     total           
time   seconds   seconds    calls  us/call  us/call  name    
79.86      4.72     4.72    10001   471.94   577.04  force
17.11      5.73     1.01 346714668     0.00     0.00  pbc
1.36      5.81     0.08    10000     8.01     8.01  velverlet_1
0.68      5.85     0.04    30006     1.33     1.33  azzero
0.68      5.89     0.04    10000     4.00     4.00  velverlet_2
0.34      5.91     0.02    10001     2.00     2.00  ekin
0.00      5.91     0.00      101     0.00     0.00  output
0.00      5.91     0.00       12     0.00     0.00  get_a_line
0.00      5.91     0.00        1     0.00     0.00  allocate_mdsys
0.00      5.91     0.00        1     0.00     0.00  free_mdsys
0.00      5.91     0.00        1     0.00     4.00  set_ic
0.00      5.91     0.00        1     0.00     0.00  set_mdsys
```
The output given by gprof showed that:
* Use of pbc() is convenient, but costs 17% (VS 25%)=> compiling with -O3
should inline it
* Loops should be unrolled for superscalar CPUs => compiling with -O3
should do it for us

## Compiler Optimization
### Use of -O3 --> PLOTTARE CALL-GRAPH
After checking the energy conservation using ```make check```,  
* Time now: 20 s (VS 39s) (1.4x faster)
* gprof was run again to see the changes in the profiling; it can be noticed
that
* * velverlet_1, ekin, output, allocate_mdsys, free_mdysys, set_ic and set_mdsys
seem not to be called 
* * more time is spent inside pbc
?????
```
%   cumulative   self              self     total           
time   seconds   seconds    calls  ns/call  ns/call  name    
71.73      1.21     1.21                             force
25.79      1.65     0.44 346714668     1.26     1.26  pbc
 1.48      1.67     0.03    30006   834.65   834.65  azzero
 1.19      1.69     0.02                             velverlet_2
 0.00      1.69     0.00       12     0.00     0.00  get_a_line
```
### -ffast-math -fexpensive-optimizations -msse3 --> PLOTTARE CALL-GRAPH
After checking again the energy conservation using ```make check```,  
* Time now: 8 s (VS 10) (3.5x faster)
* the output of gprof was the following:
```
   %   cumulative   self              self     total           
 time   seconds   seconds    calls  ns/call  ns/call  name    
 84.78      3.64     3.64                             force
 14.60      4.26     0.63 346714668     1.81     1.81  pbc
  0.35      4.28     0.02    30006   500.85   500.85  azzero
  0.23      4.29     0.01                             ekin
  0.23      4.30     0.01                             velverlet_2
  0.00      4.30     0.00       12     0.00     0.00  get_a_line
```
* Compare to LAMMPS: 3.6s => need to do more


(## Use physics
Newton's 3 rd law: F ij = -F ji
Time now: 5.4s (9.0x faster))

## More Modifications
Avoid expensive math: pow(), sqrt(), division => 108 atoms: 5 s (VS 4.0s)
(5.6x faster)



###

(4) Improvements So Far
* Use the optimal compiler flags => ~5x faster but some of it: inlining, unrolling could be coded
* Use our knowledge of physics => ~2x faster since we need to compute only half the data.
* Use our knowledge of computer hardware => 1.35x faster. (there could be more: SSE)
We are within 10% (4s vs. 3.6s) of LAMMPS.
* Try a bigger system: 2916 atoms, 100 steps Our code: 13.3s LAMMPS: 2.7s => Bad scaling with system size

(5) Making it Scale with System Size
* We compute all distances between pairs
* But for larger systems not all pairs contribute yet our effort is O(N 2 )
* Avoid distant pairs
* * Divide system in cells of size >= cutoff.
* * Sort atoms into cells
* * Look only at 26 cells around central cell

(6) The Cell-List Variant
* At startup build a list of lists to store atom indices for atoms that “belong” to a cell
* Compute a list of pairs between cells which contain atoms within cutoff. Doesn't change!
* During MD sort atoms into cells
* Then loop over list of “close” pairs of cells i and j
* For pair of cells loop over pairs of atoms in them
* Now we have linear scaling with system size at the cost of using more memory and an O(N) sort
