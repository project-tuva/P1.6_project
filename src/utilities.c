/*
utilities for main function in ljmd.c
- azzero: set array's elements to zero
- pbc: periodic boundary conditions
- ekin: compute kinetic energy
- allocate_mdsys
- free_mdsys 
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

void allocate_mdsys(mdsys_t *sys){
/*
alternative policy to avoid code replication:
add and implement nstep=natoms/size to serial sys struct and use the same function
for both the serial and the parallel case (size==1 for the serial one)
*/



#ifdef _MPI
  sys->rx=(double *)malloc(sys->nsize*sizeof(double));
  sys->ry=(double *)malloc(sys->nsize*sizeof(double));
  sys->rz=(double *)malloc(sys->nsize*sizeof(double));
  sys->vx=(double *)malloc(sys->nsize*sizeof(double));
  sys->vy=(double *)malloc(sys->nsize*sizeof(double));
  sys->vz=(double *)malloc(sys->nsize*sizeof(double));
  sys->fx=(double *)malloc(sys->nsize*sizeof(double));
  sys->fy=(double *)malloc(sys->nsize*sizeof(double));
  sys->fz=(double *)malloc(sys->nsize*sizeof(double));
  //memory for cx cy cz 
  sys->cx=(double *)malloc(sys->nsize*sizeof(double));
  sys->cy=(double *)malloc(sys->nsize*sizeof(double));
  sys->cz=(double *)malloc(sys->nsize*sizeof(double));
#else
  sys->rx=(double *)malloc(sys->natoms*sizeof(double));
  sys->ry=(double *)malloc(sys->natoms*sizeof(double));
  sys->rz=(double *)malloc(sys->natoms*sizeof(double));
  sys->vx=(double *)malloc(sys->natoms*sizeof(double));
  sys->vy=(double *)malloc(sys->natoms*sizeof(double));
  sys->vz=(double *)malloc(sys->natoms*sizeof(double));
  sys->fx=(double *)malloc(sys->natoms*sizeof(double));
  sys->fy=(double *)malloc(sys->natoms*sizeof(double));
  sys->fz=(double *)malloc(sys->natoms*sizeof(double));
#endif /*defined _MPI*/
}

void free_mdsys(mdsys_t *sys){

  free(sys->rx);
  sys->rx=NULL;
  free(sys->ry);
  sys->ry=NULL;
  free(sys->rz);
  sys->rz=NULL;
  free(sys->vx);
  sys->vx=NULL;
  free(sys->vy);
  sys->vy=NULL;
  free(sys->vz);
  sys->vz=NULL;
  free(sys->fx);
  sys->fx=NULL;
  free(sys->fy);
  sys->fy=NULL;
  free(sys->fz);
  sys->fz=NULL;
#ifdef _MPI
  free(sys->cx);
  sys->cx=NULL;
  free(sys->cy);
  sys->cy=NULL;
  free(sys->cz);
  sys->cz=NULL;
#endif /*defined _MPI*/

}



