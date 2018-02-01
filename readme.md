# N-BODY LJMD REFACTORING, OPTIMIZATION AND PARALLELIZATION PROJECT
## Group 2
The list of the group members and the names will are the following:
* Farida Farsian (ffarsian)
* Elisa Bortoli (ElisaBortoli)
* Timoteo Colnaghi (project-tuva)
* Luca Ciuffreda (ciuffredaluca)

## Introduction
The code contains the implementation of the he LJ Model for Liquid Argon.
The tasks are the following:
__1. PYTHON INTERFACE__ (Farida)
Create a python interface so that top-level operations are implemented in
python and only time critical steps are performed in C.
This can be done incrementally. Minimal goal is to replace
input (of parameters) and do unit testing in python.

__2. CODE OPTIMIZATION__ (Elisa)
Optimize the force computation: refactor the code for better optimization
and to avoid costly operations or redundant work.
Adapt data structures as needed. Document improvements with
profiling/benchmark data.

__3. MPI__ (Timoteo)
Add MPI parallelization. Document the parallel efficiency of changes.

__4. OpenMP__ (Luca)
Add MPI parallelization. Document the parallel efficiency of changes.

## Description
At first, the code was optimized using the compiler flags -O3 and -ffast-math
and then it was rewritten avoiding expensive math operations (sqrt and pow)
and applying the Newton's 3rd law.
In the following graph the execution time of each optimization step
is plotted. Each step contains the previous optimization and a new feature:

1. No optimization
2. -O3 compiler flag
3. flags -ffast-math compiler
4. expensive math avoided
5. Newton's 3rd law
6. clang compiler
7. function pbc inlined

Further details of the optimization are described in the "readme_elisa.md".

![local_time_optim](https://user-images.githubusercontent.com/23551722/35654496-b76e840e-06ed-11e8-85b7-f22c2c60a1aa.png)

Then a python interface for this serial version was implemented
(for details see "readme_farida.md").

Then the most optimized code was parallelized using OpenMP, MPI and then both
of them ("hybrid"). 

The results can be summarized by the the following graph, in which
the different times per cycle (time/n° of MD steps)
for the different implementation are reported.

![ulysses_timecycle](https://user-images.githubusercontent.com/23551722/35653743-e8fe0282-06e9-11e8-97a1-84ed40ebe9af.png)

It can noticed that for a small n° of atoms the 4 implementations are very
similar but increasing the size the performance is much better with the use
of MPI and the hybrid version.