# INDIVIDUAL PART
## Introduction
This section describes the code optimization.
The main goal is to optimize the force computation, refactoring the code in
order to avoid costly operations or redundant work.
The improvements are documented with profiling/benchmark data.
After each step, the command ```make check``` was run in order to
ensure the correctness of the results.

All the times were recorded on my local pc, an ASUS N-552VW with a CPU
Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz.

After breaking down the single file ljmd.c into multiple files and updating
the make process accordingly so that dependencies are properly applied,
__gprof__ was used to produce the call-tree.

The call-tree:
* is a control flow graph that exhibits the calling relationship among
routines in a program;
* reports the cumulative number of calls.
* a node in the graph represents a routine while an edge represent a calling
relationship.
------------------------------------------------------------------------
### Case 1: no optimization
Compiler flags:
```
CC=gcc
CFLAGS=-Wall -std=c99 -I$(HEADDIR)
LDLIBS=-lm -L$(LIBDIR) -Wl,-rpath,Obj-serial -Wl,-rpath,../Obj-serial
```

Time:
* 108 atoms: 27.871 s
* 2916 atoms: 542.359 s (9.03 min)

Profiling:
* all the functions are visible;
* pbc is the most called function.
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
### Case 2: Optimization -O3
Knowing that the optimization:
* O0: allows the compiler tries to reduce code size and execution time;
* O2: optimizes even more, turning on all optimization flags specified by -O0
and using also other other optimization flags;
* O3: turns on all optimizations specified by -O2 and also other optimization flags,
among which there are:
**-finline-functions", that considers all functions for inlining,
even if they are not declared inline.
** -fexpensive-optimizations, that performs a number of minor optimizations that are relatively expensive.

Compiler flags:
```
CC=gcc
CFLAGS=-Wall -std=c99 -O3 -I$(HEADDIR)
LDLIBS=-lm -L$(LIBDIR) -Wl,-rpath,Obj-serial -Wl,-rpath,../Obj-serial
```

Time:
* 108 atoms: 20.071 s (x 1.38 faster)
* 2916 atoms: 445.366 s  (x 1.21 faster, 7.42 min)

Profiling:
* some functions are automatically inlined (but not pbc);
* the number of calls of pbc doesn't change.
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
### Case 3: Optimizations -O3 and -ffast-math
Compiler flags:
```
CC=gcc
CFLAGS=-Wall -std=c99 -O3 -ffast-math -I$(HEADDIR)
LDLIBS=-lm -L$(LIBDIR) -Wl,-rpath,Obj-serial -Wl,-rpath,../Obj-serial
```
Time:
* 108 atoms: 6.008 s (x 4.63 faster)
* 2916 atoms: 342.196 s ( x 1.30 faster, 5.70 min)

Profiling:
* velverlet_1 is inlined
* the number of calls don't change (as expected).
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
### Case 4: Optimizations -O3 and -ffast-math + math modifications
In this case the number of expensive math operations (pow) were avoided (or at least reduced).
```
 // Constants
  double c12 = 4.0*sys->epsilon*pow(sys->sigma, 12.0);
  double c6 = 4.0*sys->epsilon*pow(sys->sigma, 6.0);
  
   // Cutoff distance squared
  double rcsq = sys->rcut* sys->rcut;
...
// Distance squared
      double rsq = rx*rx + ry*ry + rz*rz;
      
      /* compute force and energy if within cutoff */
      if (rsq < rcsq) {
	double rinv = 1.0/rsq;
	double r6 = rinv*rinv*rinv;
	
	ffac = (12.0*c12*r6-6.0*c6)*r6*rinv;
	sys->epot += 0.5*r6*(c12*r6-c6);
	
	sys->fx[i] += rx*ffac;
	sys->fy[i] += ry*ffac;
	sys->fz[i] += rz*ffac;
```

Compiler flags:
```
CC=gcc
CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -I$(HEADDIR)
LDLIBS=-lm -pg -g -L$(LIBDIR) -Wl,-rpath,Obj-serial -Wl,-rpath,../Obj-serial
```

Time:
* 108 atoms: 5.478 s (x 5.08 faster)
* 2916 atoms: 335.193 s (x 1.61 faster)

Profiling:
* no changes (as expected)
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
### Case 5: Optimizations -O3 and -ffast-math + math modifications + Newton's 3rd law
The code changes as follows:
```
 for(i=0; i < (sys->natoms)-1; ++i) {
    for(j=i+1; j < (sys->natoms); ++j) {
            
      /* get distance between particle i and j */
      rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      double rsq = rx*rx + ry*ry + rz*rz;
      
      /* compute force and energy if within cutoff */
      if (rsq < rcsq) {
	double rinv = 1.0/rsq;
	double r6 = rinv*rinv*rinv;
	
	ffac = (12.0*c12*r6-6.0*c6)*r6*rinv;
	sys->epot += r6*(c12*r6-c6);
	
	sys->fx[i] += rx*ffac; sys->fx[j] -= rx*ffac;
	sys->fy[i] += ry*ffac; sys->fy[j] -= ry*ffac;
	sys->fz[i] += rz*ffac; sys->fz[j] -= rz*ffac;
      }
    }
  }
  ```

Compiler flags:
```
CC=gcc
CFLAGS=-Wall -O3 -ffast-math -pg -g -I$(HEADDIR)
LDLIBS=-lm -pg -g -L$(LIBDIR) -Wl,-rpath,Obj-serial -Wl,-rpath,../Obj-serial
```
Time:
* 108 atoms: 2.794 s  (x 9.97 faster)
* 2916 atoms: 204.061 s (x 2.65 faster)

Profiling:
* the number of calls of pbc decreased by half (as expected)
* the function velverlet_2 seems to be inlined
```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ns/call  ns/call  name    
 74.85      0.65     0.65                             force
 23.03      0.85     0.20 173357334     1.16     1.16  pbc
  2.30      0.87     0.02    30006   667.77   667.77  azzero
  0.00      0.87     0.00       12     0.00     0.00  get_a_line

```
Comparing to LAMMPS:
* natoms = 108 --> 3.6 s => 22% less 
* natoms = 2912 --> 2.7 s => NOT done yet
------------------------------------------------------------------------
### Case 6: clang
The compiler was changed from gcc to clang.
Compiler flags:
```
CC=clang
CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -I$(HEADDIR)
LDLIBS=-lm -pg
```

Time:
* 108 atoms: 2.932 s  (x 9.50 faster)
* 2916 atoms: 177.513 (x 3.05 faster)

Profiling:
* no function seems to be inlined
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
### Case 8: pbc inlined
The function pbc was removed from the header file and then declared and defined 
in the force_compute.c, so that the function force can know it at compile time.

Compiler flags:
```
CC=gcc
CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
LDLIBS=-lm -pg -no-pie
```

Time:
* 108 atoms: 1.201 s ( x 23.20 faster)
* 2916 atoms: 35.340 s ( x 15.34 faster)

Profiling: 
* pbc is inlined
```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
100.18      1.18     1.18                             force
  0.00      1.18     0.00    30006     0.00     0.00  azzero
  0.00      1.18     0.00       12     0.00     0.00  get_a_line

```
------------------------------------------------------------------------
## Concluding
A big improvement was obtained from the not optimized version to the last case.
In the following graph the time outputs for the different optimization steps
are reported.
![time_optim](https://user-images.githubusercontent.com/23551722/35595436-060cbcd4-0617-11e8-9019-a0e1b9dbcffe.png)
Anyway some further improvement could be applied:
* Analyzing the scaling with system size (output times for several sizes)
* Apply the cell list variant (change the algorithm)
------------------------------------------------------------------------
## References
Axel Kohlmeyer, "A simple LJ many-body simulator - Optimization and Parallelization"

https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
