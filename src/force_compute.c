// Case 8
/*force_compute.c*/
#include <ljmd.h>
#include <math.h>
#include<omp.h>

static double pbc(double x, const double boxby2){
  while (x >  boxby2) x -= 2.0*boxby2;
  while (x < -boxby2) x += 2.0*boxby2;
  return x;
}

/* compute forces */
void force(mdsys_t *sys){

  double ffac;
  double rx,ry,rz,rsq,rinv,r6,*fx,*fy,*fz;
  int i,j;

  /* zero energy and forces */
  sys->epot=0.0;
  azzero(sys->fx,sys->natoms);
  azzero(sys->fy,sys->natoms);
  azzero(sys->fz,sys->natoms);
  sys->nthreads=1;
  double epot=0;
  int tid=0;

  double c12 = 4.0*sys->epsilon*pow(sys->sigma, 12.0);
  double c6 = 4.0*sys->epsilon*pow(sys->sigma, 6.0);
  double rcsq = sys->rcut* sys->rcut;

#if defined(_OPENMP)
#pragma omp parallel reduction(+:epot) private(i,j,ffac,rsq,rx,ry,rz,rinv,r6)
  {
    tid=omp_get_thread_num();
#endif
 
#pragma omp for private(i,j)
    for(i=0; i < sys->natoms; ++i){
      for(j=0;j<sys->natoms;++j){
	if (i==j) continue;
	rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
	ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
	rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
	rsq = rx*rx + ry*ry + rz*rz;

	if (rsq < rcsq) {
	  rinv = 1.0/rsq;
	  r6 = rinv*rinv*rinv;
	    
	  ffac = (12.0*c12*r6-6.0*c6)*r6*rinv;

	  epot += 0.5*r6*(c12*r6-c6);

	  sys->fx[i] += rx*ffac;
	  sys->fy[i] += ry*ffac;
	  sys->fz[i] += rz*ffac;
	  
	}
      }
    }


#if defined(_OPENMP)
  }
#endif
  
  sys->epot=epot;

}

  
  
