// Case 8
/*force_compute.c*/
#include <ljmd.h>
#include <math.h>
#include<omp.h>

#ifdef _MPI
#include <mpi.h>
#endif

#include<omp.h>


static double pbc(double x, const double boxby2){
  while (x >  boxby2) x -= 2.0*boxby2;
  while (x < -boxby2) x += 2.0*boxby2;
  return x;
}

/* compute forces */
void force(mdsys_t *sys){


  int size=1;
  int rank=0;

#ifdef _MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif /*_MPI*/

    double rsq,ffac;
    double rx,ry,rz;

    double c12 = 4.0*sys->epsilon*pow(sys->sigma, 12.0);
    double c6 = 4.0*sys->epsilon*pow(sys->sigma, 6.0);
    double rcsq = sys->rcut* sys->rcut;
    double rinv, r6;
    //    double rsq;

    /* zero energy and forces */
    sys->epot=0.0;
#ifdef _MPI
    azzero(sys->cx,sys->natoms);
    azzero(sys->cy,sys->natoms);
    azzero(sys->cz,sys->natoms);
    MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm);
    MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm);
    MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm);
#endif
    if(rank==0){
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);
    }


    double epot=0;

    /*each process computes the force for natoms/size(+1 if neeed) particles*/
    for(int i=rank; i < (sys->natoms)-1; i+=size) {
      epot=0;
#if defined(_OMP)
#pragma omp parallel reduction(+:epot) private(j,rx,ry,rz,rsq,rinv,r6,ffac)
      {
#pragma omp for
#endif
      for(int j=i+1; j < (sys->natoms); ++j) {
            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
            ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
            rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
	    //    r = sqrt(rx*rx + ry*ry + rz*rz);
	    rsq = rx*rx + ry*ry + rz*rz;

            /* compute force and energy if within cutoff */
            if (rsq < rcsq) {
	      rinv = 1.0/rsq;
	      r6 = rinv*rinv*rinv;
	      
	      ffac = (12.0*c12*r6-6.0*c6)*r6*rinv;
	      epot += r6*(c12*r6-c6);

	      /*ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r
                                         +6*pow(sys->sigma/r,6.0)/r);

                sys->epot += 4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
                                               -pow(sys->sigma/r,6.0));
	      */
#ifdef _MPI
		sys->cx[i] += rx*ffac;
                sys->cy[i] += ry*ffac;
		sys->cz[i] += rz*ffac;
                sys->cx[j] -= rx*ffac;
                sys->cy[j] -= ry*ffac;
                sys->cz[j] -= rz*ffac;
#else
                sys->fx[i] += rx*ffac;
                sys->fy[i] += ry*ffac;
                sys->fz[i] += rz*ffac;
                sys->fx[j] -= rx*ffac;
                sys->fy[j] -= ry*ffac;
		sys->fz[j] -= rz*ffac;
#endif


            } /*end of if r<rcut*/
        } /*end of for cycle on j*/
#if defined(_OMP)
      }
#endif

      sys->epot+=epot;

    } /*end of for cycle on i*/

#ifdef _MPI
    MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
    MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
    MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
    epot = sys->epot;
    MPI_Reduce(&epot, &(sys->epot), 1, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
#endif


}

  
 
  
