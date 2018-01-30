// Case 4
#include <ljmd.h>

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
        sys->ekin += 0.5*mvsq2e*sys->mass*(sys->vx[i]*sys->vx[i] +
					   sys->vy[i]*sys->vy[i] +
					   sys->vz[i]*sys->vz[i]);
    }
    sys->temp = 2.0*sys->ekin/(3.0*sys->natoms-3.0)/kboltz;
}

void allocate_mdsys(mdsys_t *sys){

  sys->rx=(double *)malloc(sys->natoms*sizeof(double));
  sys->ry=(double *)malloc(sys->natoms*sizeof(double));
  sys->rz=(double *)malloc(sys->natoms*sizeof(double));
  sys->vx=(double *)malloc(sys->natoms*sizeof(double));
  sys->vy=(double *)malloc(sys->natoms*sizeof(double));
  sys->vz=(double *)malloc(sys->natoms*sizeof(double));
  sys->fx=(double *)malloc(sys->natoms*sizeof(double));
  sys->fy=(double *)malloc(sys->natoms*sizeof(double));
  sys->fz=(double *)malloc(sys->natoms*sizeof(double));

}

void free_mdsys(mdsys_t *sys){

  free(sys->rx);
  free(sys->ry);
  free(sys->rz);
  free(sys->vx);
  free(sys->vy);
  free(sys->vz);
  free(sys->fx);
  free(sys->fy);
  free(sys->fz);

}

double cclock()
{
  struct timeval tmp;
  double sec;
  gettimeofday( &tmp, (struct timezone *)0 );
  sec = tmp.tv_sec + ((double)tmp.tv_usec)/1000000.0;
  return sec;
}
