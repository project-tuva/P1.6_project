/*force_compute.c*/
#include <ljmd.h>
#include <math.h>
#include<omp.h>

/* compute forces */
void force(mdsys_t *sys){
  double r,ffac;
  double rx,ry,rz,*fx,*fy,*fz,rxI,ryI,rzI;
  int i=0,j=0;

  /* zero energy and forces */
  sys->epot=0.0;
  azzero(sys->fx,sys->natoms);
  azzero(sys->fy,sys->natoms);
  azzero(sys->fz,sys->natoms);

  sys->nthreads=1;
  double epot=0;
  int tid=0;

#if defined(_OPENMP)
#pragma omp parallel reduction(+:epot) private(i,j,ffac,r,rx,ry,rz)
  {
    tid=omp_get_thread_num();
#endif
 
#pragma omp for private(i,j)
    for(i=0; i < sys->natoms; ++i){
      for(j=0;j<sys->natoms;++j){
	if (i!=j){
	  rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
	  ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
	  rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
	  r = sqrt(rx*rx + ry*ry + rz*rz);

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
    }


#if defined(_OPENMP)
  }
#endif
  
  sys->epot=epot;
  
}

  
  
