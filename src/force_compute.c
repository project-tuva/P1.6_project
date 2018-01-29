/*force_compute.c*/
#include <ljmd.h>
#include <math.h>
#include<omp.h>

/* compute forces */
void force(mdsys_t *sys){
    double r,ffac;
    double rx,ry,rz,*fx,*fy,*fz;
    int i,j,tid=0;

    /* zero energy and forces */
    sys->epot=0.0;
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);

    sys->nthreads=1;
    double epot=0;
    
#if defined(_OPENMP)
#pragma omp parallel reduction(+:epot) {
    tid=omp_get_thread_num();
    sys->nthreads=omp_get_num_threads();
#endif    

    fx=sys->fx+(tid*sys->natoms);
    fy=sys->fy+(tid*sys->natoms);
    fz=sys->fz+(tid*sys->natoms);
#if defined(_OPENMP)
#pragma omp for {
#endif    
    for(i=0; i < (sys->natoms) -1; i+=sys->nthreads) {
      int ii=i+tid;
      if(ii>=(sys->natoms) -1) break;
      for(int j=0; j < (sys->natoms); ++j) {

	/* particles have no interactions with themselves */
	if (i==j) continue;

	/* get distance between particle i and j */
	rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
	ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
	rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
	r = sqrt(rx*rx + ry*ry + rz*rz);

	/* compute force and energy if within cutoff */
	if (r < sys->rcut) {
	  ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r
				    +6*pow(sys->sigma/r,6.0)/r);

	  epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
					-pow(sys->sigma/r,6.0));

	  sys->fx[i] += rx/r*ffac;
	  sys->fy[i] += ry/r*ffac;
	  sys->fz[i] += rz/r*ffac;
	}
      }
    }

    if(tid=0)
      sys->epot=epot;
    
#if defined(_OPENMP)
      }
    }
#endif    
    
    
}
