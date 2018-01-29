## Case 1: gcc std=c99
* Time: 27.871 s
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

* Time: 19.542 s (x1.42 faster)

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
* Time: 6.008 s (x4.63 faster)
```
 %   cumulative   self              self     total           
 time   seconds   seconds    calls  ns/call  ns/call  name    
 80.46      1.59     1.59                             force
 18.47      1.96     0.37 346714668     1.05     1.05  pbc
  0.76      1.97     0.02    30006   500.85   500.85  azzero
  0.51      1.98     0.01                             velverlet_2
  0.00      1.98     0.00       12     0.00     0.00  get_a_line
  
```
The command ```make check``` was successfully executed.

------------------------------------------------------------------------
## Case 4: Optimizations -O3 and -ffast-math + math modifications
* Time: 5.478 s (x5.08 faster)
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
* Time: 2.794 s  (x9.97 faster)
```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ns/call  ns/call  name    
 74.85      0.65     0.65                             force
 23.03      0.85     0.20 173357334     1.16     1.16  pbc
  2.30      0.87     0.02    30006   667.77   667.77  azzero
  0.00      0.87     0.00       12     0.00     0.00  get_a_line

```
The command ```make check``` was successfully executed.

Comparing to LAMMPS:
natoms = 108 --> 3.6 s => 22% less :)
natoms = 2912 --> 2.7 s => NOT done yet
------------------------------------------------------------------------
## Case 6: Inline? :(

* Time: 2.801 s (worse than Case 5, x9.95 faster)
The command ```make check``` was successfully executed.


https://www.greenend.org.uk/rjk/tech/inline.html
"
...
(3) A C99 model.
Use inline in a common header, and
provide definitions in a .c file somewhere, via extern declarations.

For instance, in the header file:
inline int max(int a, int b) {
  return a > b ? a : b;
}
...and in exactly one source file:
#include "header.h"
extern int max(int a, int b);
In file included from ../src/verlet_2.c:3:0:
../include/ljmd.h:45:15: warning: inline function ‘pbc’ declared but
never defined
 inline double pbc(double x, const double boxby2);

To support legacy compilers, you have to swap the whole thing around so that
- the declarations are visible in the common header and ("inline")
- the definitions are restricted to a single translation unit,
with inline defined away. ("extern")????
"

I swapped like that:
-header (declar) --> extern double pbc(double x, const double boxby2);
-src file (defin) --> inline double pbc(double x, const double boxby2){...

In this case it works but I think it's not inlined.
```
 %   cumulative   self              self     total           
 time   seconds   seconds    calls  ns/call  ns/call  name    
 70.40      0.52     0.52                             force
 25.72      0.71     0.19 173357334     1.10     1.10  pbc         !!!!!
  2.71      0.73     0.02    30006   667.77   667.77  azzero
  1.35      0.74     0.01                             velverlet_1
  0.00      0.74     0.00       12     0.00     0.00  get_a_line

```
------------------------------------------------------------------------
## Case 7: clang :(
* Time: 2.932 s  (even worse than Case 5, x9.50 faster)

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
  Is not inline anything??
------------------------------------------------------------------------
## Case 8: g++ :(

https://stackoverflow.com/questions/172587/what-is-the-difference-between-g-and-gcc
gcc and g++ are compiler-drivers of the 'Gnu Compiler Collection'
(which was once upon a time just the 'Gnu C Compiler').
Even though they automatically determine which backends (cc1 cc1plus ...) to call
depending on the file-type, unless overridden with -x language,
they have some differences.
The probably most important difference in their defaults is which libraries
they link against automatically.
According to [1] and [2], g++ is equivalent to
gcc -xc++ -lstdc++ -shared-libgcc
(the 1st is a compiler option, the 2nd two are linker options).
This can be checked by running both with the -v option
(it displays the backend toolchain commands being run).

* Time: 2.804 s (worse than Case 5, x9.93 faster)
------------------------------------------------------------------------
## Case 9: icc (intel compiler)
### c3e cluster
module  list 
Currently Loaded Modulefiles:
  1) gnu/4.8.3    2) intel/15.0

[ebortoli@b22 P1.6_project]$ make
make  -C Obj-serial
make[1]: Entering directory `/u/MHPC17/ebortoli/P1.6_project/Obj-serial'
icc -c ../src/utilities.c -Wall -O3 -ffast-math -pg -g -fno-pie -I../include -o ../src/utilities.o  

Error: A license for CCompL is not available (-76,61026,2).

License file(s) used were (in this order):
    1.  Trusted Storage
**  2.  /u/MHPC17/ebortoli/intel/licenses
**  3.  /opt/intel/licenses
**  4.  /opt/cluster/intel/composer_xe_2015.0.090/licenses
**  5.  /opt/cluster/intel/composer_xe_2015.0.090/licenses/licenses13.lic
**  6.  /opt/cluster/intel/composer_xe_2015.0.090/bin/intel64/../../Licenses
**  7.  /u/MHPC17/ebortoli/Licenses
**  8.  /Users/Shared/Library/Application Support/Intel/Licenses
**  9.  /opt/cluster/intel/composer_xe_2015.0.090/bin/intel64/*.lic

Please visit http://software.intel.com/sites/support/ if you require technical assistance.

icc: error #10052: could not checkout FLEXlm license
make[1]: *** [../src/utilities.o] Error 1
make[1]: Leaving directory `/u/MHPC17/ebortoli/P1.6_project/Obj-serial'
make: *** [serial] Error 2

### Ulysses cluster (login node)
* Time: 3.832 s (worse than Case 5, x7.27 faster)

[ebortoli@login1 P1.6_project]$ make 
make  -C Obj-serial
make[1]: Entering directory `/scratch/ebortoli/P1.6_project/Obj-serial'
icc -c ../src/force_compute.c -Wall -O3 -ffast-math -pg -g -fno-pie -I../include -o ../src/force_compute.o  
icc: command line warning #10006: ignoring unknown option '-ffast-math'
icc -c ../src/input.c -Wall -O3 -ffast-math -pg -g -fno-pie -I../include -o ../src/input.o  
icc: command line warning #10006: ignoring unknown option '-ffast-math'
icc -c ../src/output.c -Wall -O3 -ffast-math -pg -g -fno-pie -I../include -o ../src/output.o  
icc: command line warning #10006: ignoring unknown option '-ffast-math'
icc -c ../src/utilities.c -Wall -O3 -ffast-math -pg -g -fno-pie -I../include -o ../src/utilities.o  
icc: command line warning #10006: ignoring unknown option '-ffast-math'
icc -c ../src/verlet_1.c -Wall -O3 -ffast-math -pg -g -fno-pie -I../include -o ../src/verlet_1.o  
icc: command line warning #10006: ignoring unknown option '-ffast-math'
icc -c ../src/verlet_2.c -Wall -O3 -ffast-math -pg -g -fno-pie -I../include -o ../src/verlet_2.o  
icc: command line warning #10006: ignoring unknown option '-ffast-math'
icc -c ../main.c -Wall -O3 -ffast-math -pg -g -fno-pie -I../include -o ../main.o
icc: command line warning #10006: ignoring unknown option '-ffast-math'
icc -o ../ljmd-serial.x -Wall -O3 -ffast-math -pg -g -fno-pie -I../include ../src/force_compute.o ../src/input.o ../src/output.o ../src/utilities.o ../src/verlet_1.o ../src/verlet_2.o ../main.o -lm -pg -fno-pie
icc: command line warning #10006: ignoring unknown option '-ffast-math'
make[1]: Leaving directory `/scratch/ebortoli/P1.6_project/Obj-serial'
------------------------------------------------------------------------
## Case 10: gcc (default)
* Time: 2.784 s (WINNER!!! x10.01 faster)

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