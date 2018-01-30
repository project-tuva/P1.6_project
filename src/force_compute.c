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
    
#if defined(_OPENMP)
#pragma omp parallel reduction(+:epot) private(i,j,rxI,ryI,rzI,r,rx,ry,rz)
#endif
  {
#if defined(_OPENMP)
    int tid=omp_get_thread_num();
#else
    int tid=0;
#endif
    for(i=0; i < sys->natoms-1; i += sys->nthreads){
      int ii=i+tid;
      if (ii >=sys->natoms) break;
      for(j=0;j<sys->natoms;++j){
	if (ii==j) continue;
	rx=pbc(sys->rx[ii] - sys->rx[j], 0.5*sys->box);
	ry=pbc(sys->ry[ii] - sys->ry[j], 0.5*sys->box);
	rz=pbc(sys->rz[ii] - sys->rz[j], 0.5*sys->box);
	r = sqrt(rx*rx + ry*ry + rz*rz);

	if (r < sys->rcut) {
	  ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r
				    +6*pow(sys->sigma/r,6.0)/r);

	  epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
					-pow(sys->sigma/r,6.0));

	  sys->fx[ii] += rx/r*ffac;
	  sys->fy[ii] += ry/r*ffac;
	  sys->fz[ii] += rz/r*ffac;
	}
      }
    }
  }

  sys->epot=epot;

/*   int tid=0; */

/* #if defined(_OPENMP) */
/* #pragma omp parallel reduction(+:epot) */
/*   { */
/*     tid=omp_get_thread_num();  */
/* #endif */
 
/* #pragma omp for private(i,j) */
/*     for(i=0; i < sys->natoms-1; ++i){ */
/*       //int ii=i+tid; */
/*       //if (ii >=sys->natoms) break; */
/*       for(j=0;j<sys->natoms;++j){ */
/* 	if (i!=j){ */
/* 	  rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box); */
/* 	  ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box); */
/* 	  rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box); */
/* 	  r = sqrt(rx*rx + ry*ry + rz*rz); */

/* 	  if (r < sys->rcut) { */
/* 	    ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r */
/* 				      +6*pow(sys->sigma/r,6.0)/r); */

/* 	    epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0) */
/* 					  -pow(sys->sigma/r,6.0)); */

/* 	    sys->fx[i] += rx/r*ffac; */
/* 	    sys->fy[i] += ry/r*ffac; */
/* 	    sys->fz[i] += rz/r*ffac;	    */
/* 	  } */
/* 	} */
/*       } */
/*     }  */


/* #if defined(_OPENMP) */
/*   } */

/* #endif */
  
/* sys->epot=epot; */
  
}

  
  
