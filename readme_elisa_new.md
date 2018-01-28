## Case 1: No optimization
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
Comparing to LAMMPS:
natoms = 108 --> 3.6 s => 34% more
natoms = 2912 --> 2.7 s => 

## Case 4: Optimizations -O3 and -ffast-math + math modifications + inline pbc
https://www.greenend.org.uk/rjk/tech/inline.html

```

```



gprof ljmd-serial.x | gprof2dot | dot -T png -o 4_callgraph.png