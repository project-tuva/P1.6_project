/*velverlet_2.c: complete verlet*/

#include <ljmd.h>

void velverlet_2(mdsys_t *sys) // velocity verlet algorithm
{
    /* second part: propagate velocities by another half step */
    for (int i=0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
        sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
        sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;
    }
}
