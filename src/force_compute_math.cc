/*force_compute.c*/
#include <ljmd.h>
#include <math.h>


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

  double c12 = 4.0*sys->epsilon*pow(sys->sigma, 12.0);
  double c6 = 4.0*sys->epsilon*pow(sys->sigma, 6.0);
  double rcsq = sys->rcut* sys->rcut;
  
  for(i=0; i < (sys->natoms); ++i) {
    for(j=0; j < (sys->natoms); ++j) {

      /* particles have no interactions with themselves */
      if (i==j) continue;
            
      /* get distance between particle i and j */
      rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
      ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
      rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
      double rsq = rx*rx + ry*ry + rz*rz;
      
      /* compute force and energy if within cutoff */
      if (rsq < rcsq) {
	double rinv = 1.0/rsq;
	double r6 = rinv*rinv*rinv;
	
	ffac = (12.0*c12*r6-6.0*c6)*r6*rinv;
	sys->epot += 0.5*r6*(c12*r6-c6);
	
	sys->fx[i] += rx*ffac;
	sys->fy[i] += ry*ffac;
	sys->fz[i] += rz*ffac;
      }
    }
  }
}
