/* ljmd.h */
#ifndef _LJMD_
#define _LJMD_

#include <stdlib.h>
#include <stdio.h>
#ifdef _MPI
#include <mpi.h>
#endif /*defined _MPI*/

// Constants
/* generic file- or pathname buffer length */
static const int BLEN=200;

/* a few physical constants */
static const double kboltz=0.0019872067;     /* boltzman constant in kcal/mol/K */
static const double mvsq2e=2390.05736153349; /* m*v^2 in kcal/mol */

// Data
/* structure to hold the complete information
 * about the MD system */
#ifndef _MPI
struct _mdsys {
  int natoms,nfi,nsteps;
  double dt, mass, epsilon, sigma, box, rcut;
  double ekin, epot, temp;
  double *rx, *ry, *rz;
  double *vx, *vy, *vz;
  double *fx, *fy, *fz;
};
#else
struct _mdsys {
  int natoms,nsize,nfi,nsteps; /*added nsize: n of atoms processed by the current process*/
  MPI_Comm mpicomm;
  double dt, mass, epsilon, sigma, box, rcut;
  double ekin, epot, temp;
  double *rx, *ry, *rz;
  double *vx, *vy, *vz;
  double *fx, *fy, *fz;
  double *cx, *cy, *cz;
};
#endif /*defined _MPI*/

typedef struct _mdsys mdsys_t;

// Methods
/* helper function: read a line and then return
   the first string with whitespace stripped off */
int get_a_line(FILE *fp, char *buf); // previously it was static

/* set structure from input */
int set_mdsys(mdsys_t *sys,char restfile[BLEN],char trajfile[BLEN],char ergfile[BLEN],char line[BLEN],int *nprint, int rank, int size);

int set_ic(mdsys_t *sys, char restfile[BLEN]);
int set_ic_f(mdsys_t *sys, char restfile[BLEN]);

/* helper function: zero out an array */
void azzero(double *d, const int n); // previously it was static

/* helper function: apply minimum image convention */
double pbc(double x, const double boxby2);

/* helper functions: malloc and free memory for r v f for all the particles */
void allocate_mdsys(mdsys_t *sys);
void free_mdsys(mdsys_t *sys);

/*helper function: evaluate nsize= num of atoms assigned to the current process*/
#ifdef _MPI
int set_nsize(int natoms, int rank, int size);
#endif /*defined _MPI*/

/* compute kinetic energy */
void ekin(mdsys_t *sys); // previously it was static

/* compute forces */
void force(mdsys_t *sys); // previously it was static

/* velocity verlet */
void velverlet_1(mdsys_t *sys); // previously it was static
void velverlet_2(mdsys_t *sys); // previously it was static

/* append data to output. */
void output(mdsys_t *sys, FILE *erg, FILE *traj); // previously it was static
void test_output(mdsys_t *sys, char * namefile);

#endif
