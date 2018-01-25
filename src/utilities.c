/*
utilities for main function in ljmd.c
- azzero: set array's elements to zero
- pbc: periodic boundary conditions
- ekin: compute kinetic energy
*/
#include <ljmd.h>

/* helper function: zero out an array */
void azzero(double *d, const int n){
    int i;
    for (i=0; i<n; ++i) {
        d[i]=0.0;
    }
}

/* helper function: apply minimum image convention */
double pbc(double x, const double boxby2){
    while (x >  boxby2) x -= 2.0*boxby2;
    while (x < -boxby2) x += 2.0*boxby2;
    return x;
}


/* compute kinetic energy */
void ekin(mdsys_t *sys){
    int i;

    sys->ekin=0.0;
    for (i=0; i<sys->natoms; ++i) {
        sys->ekin += 0.5*mvsq2e*sys->mass*(sys->vx[i]*sys->vx[i] + sys->vy[i]*sys->vy[i] + sys->vz[i]*sys->vz[i]);
    }
    sys->temp = 2.0*sys->ekin/(3.0*sys->natoms-3.0)/kboltz;
}
