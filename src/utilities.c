// Case 8
/*
utilities for main function in ljmd.c
- azzero: set array's elements to zero
- pbc: periodic boundary conditions
- ekin: compute kinetic energy
- allocate_mdsys
- free_mdsys 
*/
#include <ljmd.h>
#ifdef _MPI
#include <mpi.h>
#endif /*defined _MPI*/

#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>

/* helper function: zero out an array */
void azzero(double *d, const int n){
    int i;
    for (i=0; i<n; ++i) {
        d[i]=0.0;
    }
}


/* helper function: apply minimum image convention 
// This need to be commented for the case 8 (inline pbc)
double pbc(double x, const double boxby2){
    while (x >  boxby2) x -= 2.0*boxby2;
    while (x < -boxby2) x += 2.0*boxby2;
    return x;
}
*/

/* compute kinetic energy */
void ekin(mdsys_t *sys){ // only process 0
    int i;

    sys->ekin=0.0;
    for (i=0; i<sys->natoms; ++i) {
        sys->ekin += 0.5*mvsq2e*sys->mass*(sys->vx[i]*sys->vx[i] +
					   sys->vy[i]*sys->vy[i] +
					   sys->vz[i]*sys->vz[i]);
    }
    sys->temp = 2.0*sys->ekin/(3.0*sys->natoms-3.0)/kboltz;
}

void allocate_mdsys(mdsys_t *sys){
  int rank =0;
  //int size = 1;
#ifdef _MPI
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  //MPI_Comm_size(MPI_COMM_WORLD,&size);
#endif /*defined MPI*/

  sys->rx=(double *)malloc(sys->natoms*sizeof(double));
  sys->ry=(double *)malloc(sys->natoms*sizeof(double));
  sys->rz=(double *)malloc(sys->natoms*sizeof(double));
  if(rank==0){
    sys->vx=(double *)malloc(sys->natoms*sizeof(double));
    sys->vy=(double *)malloc(sys->natoms*sizeof(double));
    sys->vz=(double *)malloc(sys->natoms*sizeof(double));
    sys->fx=(double *)malloc(sys->natoms*sizeof(double));
    sys->fy=(double *)malloc(sys->natoms*sizeof(double));
    sys->fz=(double *)malloc(sys->natoms*sizeof(double));
  }

#ifdef _MPI
  sys->cx=(double *)malloc(sys->natoms*sizeof(double));
  sys->cy=(double *)malloc(sys->natoms*sizeof(double));
  sys->cz=(double *)malloc(sys->natoms*sizeof(double));
#endif /*defined _MPI*/
}

void free_mdsys(mdsys_t *sys){
  int rank =0;
#ifdef _MPI
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
#endif /*defined MPI*/
  free(sys->rx);
  sys->rx=NULL;
  free(sys->ry);
  sys->ry=NULL;
  free(sys->rz);
  sys->rz=NULL;
  if(rank==0){
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
  }
#ifdef _MPI
  sys->cx=NULL;
  free(sys->cy);
  sys->cy=NULL;
  free(sys->cz);
  sys->cz=NULL;
#endif
}

double cclock()
{
  struct timeval tmp;
  double sec;
  gettimeofday( &tmp, (struct timezone *)0 );
  sec = tmp.tv_sec + ((double)tmp.tv_usec)/1000000.0;
  return sec;
}
