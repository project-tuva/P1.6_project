// Case 8
/*force_compute.c*/
#include <ljmd.h>
#include <math.h>

static double pbc(double x, const double boxby2){
  while (x >  boxby2) x -= 2.0*boxby2;
  while (x < -boxby2) x += 2.0*boxby2;
  return x;
}

/* compute forces */
void force(mdsys_t *sys){

  double ffac;
  double rx,ry,rz;
  int i,j;

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
	  double rsq = rx*rx + ry*ry + rz*rz;

	  if (rsq < rcsq) {
	    double rinv = 1.0/rsq;
	    double r6 = rinv*rinv*rinv;
	    
	    ffac = (12.0*c12*r6-6.0*c6)*r6*rinv;

	    epot += r6*(c12*r6-c6);

	    sys->fx[i] += rx*ffac;
	    sys->fy[i] += ry*ffac;
	    sys->fz[i] += rz*ffac;
	  }
	}
      }
    }


#if defined(_OPENMP)
  }
#endif
  
  sys->epot=epot;

}

  
  
