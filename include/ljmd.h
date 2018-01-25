/* ljmd.h */
#ifndef _LJMD_
#define _LJMD_

// Constants
/* generic file- or pathname buffer length */
#define BLEN 200

/* a few physical constants */
const double kboltz=0.0019872067;     /* boltzman constant in kcal/mol/K */
const double mvsq2e=2390.05736153349; /* m*v^2 in kcal/mol */

// Data
/* structure to hold the complete information
 * about the MD system */
struct _mdsys {
    int natoms,nfi,nsteps;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
};
typedef struct _mdsys mdsys_t;

// Methods
/* helper function: read a line and then return
   the first string with whitespace stripped off */
int get_a_line(FILE *fp, char *buf); // previously it was static

/* helper function: zero out an array */
void azzero(double *d, const int n); // previously it was static

/* helper function: apply minimum image convention */
double pbc(double x, const double boxby2);

/* compute kinetic energy */
void ekin(mdsys_t *sys); // previously it was static

/* compute forces */
static void force(mdsys_t *sys); // previously it was static

/* velocity verlet */
void velverlet_1(mdsys_t *sys); // previously it was static
void velverlet_2(mdsys_t *sys); // previously it was static

/* append data to output. */
void output(mdsys_t *sys, FILE *erg, FILE *traj); // previously it was static

#endif
