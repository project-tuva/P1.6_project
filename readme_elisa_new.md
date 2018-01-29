# INDIVIDUAL PART
## Introduction
This section describes the code optimization.
The main goal is to optimize the force computation, refactoring the code in
order to avoid costly operations or redundant work.
The improvements are documented with profiling/benchmark data.
After each step, the command ```make check``` was run in order to
ensure the correctness of the results.

(So two steps will be perfomed:
1. Mapping, through the visualization of the call-tree and code coverage
2. Profiling, analysing the code behaviour (hotspots, bottlenecks, resource
utilization) and its efficiency
(instruction cycles/cycles, L1/L2/RAM hits, branch misses))

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

## Case 1: gcc std=c99
Compiler flags:
```

```
* Time 108: 27.871 s
* Time 2916: 542.359 s (9.03 min)

```
 %   cumulative   self              self     total           
 time   seconds   seconds    calls  us/call  us/call  name    
 79.44      4.37     4.37    10001   436.89   538.98  force
 17.47      5.33     0.96 346714668     0.00     0.00  pbc
  1.09      5.39     0.06    30006     2.00     2.00  azzero
  1.09      5.45     0.06    10000     6.01     6.01  velverlet_2
  0.91      5.50     0.05    10000     5.01     5.01  velverlet_1
  0.00      5.50     0.00    10001     0.00     0.00  ekin
  0.00      5.50     0.00      101     0.00     0.00  output
  0.00      5.50     0.00       12     0.00     0.00  get_a_line
  0.00      5.50     0.00        2     0.00     0.00  cclock
  0.00      5.50     0.00        1     0.00     0.00  allocate_mdsys
  0.00      5.50     0.00        1     0.00     0.00  free_mdsys
  0.00      5.50     0.00        1     0.00     6.00  set_ic
  0.00      5.50     0.00        1     0.00     0.00  set_mdsys
  
```
------------------------------------------------------------------------
## Case 2: Optimization -O3
Compiler flags:
```

```

* Time 108: 20.071 s (x 1.38 faster)
* Time 2916: 445.366 s  (x 1.21 faster, 7.42 min)

https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
*** O0 ***
Optimizing compilation takes somewhat more time,
and a lot more memory for a large function.
With -O, the compiler tries to reduce code size and execution time,
*** O2 ***
Optimize even more. GCC performs nearly all supported optimizations
that do not involve a space-speed tradeoff.
As compared to -O, this option increases both compilation time and the
performance of the generated code.
-O2 turns on all optimization flags specified by -O.
It also turns on the other optimization flags
*** O3 ***
Optimize yet more.
-O3 turns on all optimizations specified by -O2 and also
turns on the following optimization flags:
..
-finline-functions:
Consider all functions for inlining, even if they are not declared inline.
The compiler heuristically decides which functions are worth
integrating in this way.
If all calls to a given function are integrated, and the function is
declared static, then the function is normally not output as assembler code
in its own right.

Enabled at level -O3. 
-fexpensive-optimizations:
Perform a number of minor optimizations that are relatively expensive.
(Enabled at levels -O2, -O3, -Os.)

-fexpensive-optimizations
Perform a number of minor optimizations that are relatively expensive.
Enabled at levels -O2, -O3, -Os.

```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  us/call  us/call  name    
 78.35      1.40     1.40                             force
 19.03      1.74     0.34 346714668     0.00     0.00  pbc
  1.68      1.77     0.03    30006     1.00     1.00  azzero
  0.56      1.78     0.01                             velverlet_1
  0.56      1.79     0.01                             velverlet_2
  0.00      1.79     0.00       12     0.00     0.00  get_a_line
  
```
------------------------------------------------------------------------
## Case 3: Optimizations -O3 and -ffast-math
Compiler flags:
```

```

* Time 108: 6.008 s (x 4.63 faster)
* Time 2916: 342.196 s ( x 1.30 faster, 5.70 min)

```
 %   cumulative   self              self     total           
 time   seconds   seconds    calls  ns/call  ns/call  name    
 80.46      1.59     1.59                             force
 18.47      1.96     0.37 346714668     1.05     1.05  pbc
  0.76      1.97     0.02    30006   500.85   500.85  azzero
  0.51      1.98     0.01                             velverlet_2
  0.00      1.98     0.00       12     0.00     0.00  get_a_line
  
```
------------------------------------------------------------------------
## Case 4: Optimizations -O3 and -ffast-math + math modifications
Compiler flags:
```

```
* Time 108: 5.478 s (x 5.08 faster)
* Time 2916: 335.193 s (x 1.61 faster)

```
 %   cumulative   self              self     total           
 time   seconds   seconds    calls  us/call  us/call  name    
 67.61      1.10     1.10                             force
 29.20      1.58     0.48 346714668     0.00     0.00  pbc
  2.77      1.62     0.05    30006     1.50     1.50  azzero
  0.61      1.63     0.01                             velverlet_2
  0.00      1.63     0.00       12     0.00     0.00  get_a_line

```
------------------------------------------------------------------------
## Case 5: Optimizations -O3 and -ffast-math + math modifications + Newton
Compiler flags:
```

```

* Time 108: 2.794 s  (x 9.97 faster)
* Time 2916: 204.061 s (x 2.65 faster)

```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ns/call  ns/call  name    
 74.85      0.65     0.65                             force
 23.03      0.85     0.20 173357334     1.16     1.16  pbc
  2.30      0.87     0.02    30006   667.77   667.77  azzero
  0.00      0.87     0.00       12     0.00     0.00  get_a_line

```
Comparing to LAMMPS:
natoms = 108 --> 3.6 s => 22% less :)
natoms = 2912 --> 2.7 s => NOT done yet
------------------------------------------------------------------------
## Case 7: clang
Compiler flags:
```
CC=clang
CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -I$(HEADDIR)
LDLIBS=-lm -pg

```
* Time 108: 2.932 s  (even worse than Case 5, x 9.50 faster)
* Time 2916: 177.513 (x 3.05 faster)

```
 %   cumulative   self              self     total           
 time   seconds   seconds    calls  us/call  us/call  name    
 58.86      0.57     0.57    10001    57.09    96.14  force
 39.24      0.95     0.38 173357334     0.00     0.00  pbc
  1.03      0.96     0.01    30006     0.33     0.33  azzero
  1.03      0.97     0.01    10000     1.00     1.00  velverlet_1
  0.00      0.97     0.00    10001     0.00     0.00  ekin
  0.00      0.97     0.00    10000     0.00     0.00  velverlet_2
  0.00      0.97     0.00      101     0.00     0.00  output
  0.00      0.97     0.00       12     0.00     0.00  get_a_line
  0.00      0.97     0.00       10     0.00     0.00  set_mdsys
  0.00      0.97     0.00        2     0.00     0.00  cclock
  0.00      0.97     0.00        1     0.00     0.00  allocate_mdsys
  0.00      0.97     0.00        1     0.00     0.00  free_mdsys
  0.00      0.97     0.00        1     0.00     1.00  set_ic
  ```
------------------------------------------------------------------------
## Case 10: gcc (default)
Compiler flags:
```
CC=gcc
CFLAGS=-Wall -O3 -ffast-math -pg -g -fno-pie -I$(HEADDIR)
LDLIBS=-lm -pg -fno-pie
```
* Time 108: 2.784 s ( x 10.01 faster)
* Time 2916: 167.499 ( x 3.23 faster)

```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ns/call  ns/call  name    
 70.02      0.65     0.65                             force
 29.09      0.92     0.27 173357334     1.56     1.56  pbc
  1.08      0.93     0.01    30006   333.88   333.88  azzero
  0.00      0.93     0.00       12     0.00     0.00  get_a_line
  ```
------------------------------------------------------------------------
gprof ljmd-serial.x | gprof2dot | dot -T png -o 4_callgraph.png
------------------------------------------------------------------------
## Case 11: gcc (default) with inline
Compiler flags:
```
CC=gcc
CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
LDLIBS=-lm -pg -no-pie
```
pbc
-removed from the header
-put in force_compute to have it inline (static)

* Time 108: 1.201 s ( x 23.20 faster)
* Time 2916: 35.340 s ( x 15.34 faster)
```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
100.18      1.18     1.18                             force
  0.00      1.18     0.00    30006     0.00     0.00  azzero
  0.00      1.18     0.00       12     0.00     0.00  get_a_line

```
------------------------------------------------------------------------
## Case 12: cell-list variant
Compiler flags:
```
```



------------------------------------------------------------------------
TO DO
(1) pbc inlined --> DONE
(2) run on c3e
(3) comments on the resuts
(4) change compiler: intel flags??
(5) write the makefile in such a way we can compile with and without
debugging
(6) prefecthing --> IF I HAVE TIME
(7) measure time with 2916 particles --> ABSOLUTELY!!!
(8) cell list --> ABSOLUTELY!!!
(9) graph with times
------------------------------------------------------------------------
QUESTIONS
(0) perchè force non è profiled? Dovrebbe esserlo..occhio alla flag
(1) inline pbc: meccanismo e why it doesn't when using O3?
(2) static, extern, inline
(3) prefetching
(3) ha senso conforntare gcc e g++? NO
(4) mi dimentico flag con icc? NO

Alternativa:
definire nel header file la funzione pbc

------------------------------------------------------------------------
gprof ljmd-serial.x | gprof2dot | dot -T png -o 4_callgraph.png
------------------------------------------------------------------------