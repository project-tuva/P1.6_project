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
1. __PYTHON INTERFACE__ (Farida)
Create a python interface so that top-level operations are implemented in
python and only time critical steps are performed in C.
This can be done incrementally. Minimal goal is to replace
input (of parameters) and do unit testing in python.

2. __CODE OPTIMIZATION__ (Elisa)
Optimize the force computation: refactor the code for better optimization
and to avoid costly operations or redundant work.
Adapt data structures as needed. Document improvements with
profiling/benchmark data.

3. __MPI__ (Timoteo)
Add MPI parallelization. Document the parallel efficiency of changes.

4. __OpenMP__ (Luca)
Add MPI parallelization. Document the parallel efficiency of changes.

## Description
At first, the code was optimized using the compiler flags -O3 and -ffast-math
and then it was rewritten avoiding expensive math operations (sqrt and pow)
and applying the Newton's 3rd law.
A python interface for this serial version was implemented
(see "readme_farida.md").
(The details of the optimization are described in the "readme_elisa.md".)

Then the most optimized code was parallelized using OpenMP, MPI and then both
of them ("hybrid"). 

The results can be summarized by the the following graph, in which
the different times per cycle (time/n° of MD steps)
for the different implementation are reported.

![ulysses_timecycle](https://user-images.githubusercontent.com/23551722/35653743-e8fe0282-06e9-11e8-97a1-84ed40ebe9af.png)

It can noticed that for a small n° of atoms the 4 implementations are very
similar but increasing the size the performance is better with the use
of MPI and the hybrid version.