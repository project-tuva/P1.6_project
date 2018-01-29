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

  double c12 = 4.0*sys->epsilon*pow(sys->sigma, 12.0);
  double c6 = 4.0*sys->epsilon*pow(sys->sigma, 6.0);
  double rcsq = sys->rcut* sys->rcut;

  for(i=0; i<sys->npair; ++i){
    /*
      int ix,iy,iz;
      cell_t *c;
      i = (ix*4)+iy)*4 + iz;
      c[i].idxlist[c[i].atoms]=0;
      c[i].num++;
    */
    cell_t *c1, c2;
    c1 = sys->clist + sys->plist[2*i]; // plist???
    c2 = sys->clist + sys->plist[2*i+1]; // plist???

    for (j=0; j<c1->natoms; ++j){
      int ii = c1->idxlist[j];
      double rx1 = sys->rx[ii];
      double ry1 = sys->ry[ii];
      double rz1 = sys->rz[ii];
      
      for(k=0; k<c2->natoms; ++k) {
	double rx,ry,rz,rsq;
	rx = pbc(sys->rx1 - sys->rx[jj], boxby2, sys->box);
	ry = pbc(sys->ry1 - sys->ry[jj], boxby2, sys->box);
	rx = pbc(sys->rz1 - sys->rz[jj], boxby2, sys->box);

	double rsq = rx*rx + ry*ry + rz*rz;
	
	/* compute force and energy if within cutoff */
	if (rsq < rcsq) {
	  double rinv = 1.0/rsq;
	  double r6 = rinv*rinv*rinv;
	
	  ffac = (12.0*c12*r6-6.0*c6)*r6*rinv;
	  sys->epot += r6*(c12*r6-c6);
	
	  sys->fx[i] += rx*ffac; sys->fx[j] -= rx*ffac;
	  sys->fy[i] += ry*ffac; sys->fy[j] -= ry*ffac;
	  sys->fz[i] += rz*ffac; sys->fz[j] -= rz*ffac;
	}
      }
    }
  }
