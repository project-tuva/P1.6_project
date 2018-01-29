/*force_compute.c*/
#include <ljmd.h>
#include <math.h>
#ifdef _MPI
#include <mpi.h>
#endif

/* compute forces */
void force(mdsys_t *sys, int rank, int size){
    double r,ffac;
    double rx,ry,rz;

    /* zero energy and forces */
    sys->epot=0.0;
#ifdef _MPI
    azzero(sys->cx,sys->natoms);
    azzero(sys->cy,sys->natoms);
    azzero(sys->cz,sys->natoms);
    MPI_Bcast(sys->rx, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm);
    MPI_Bcast(sys->ry, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm);
    MPI_Bcast(sys->rz, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm);
#else
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);
#endif

    /*each process computes the force for natoms/size(+1 if neeed) particles*/
    for(int i=rank; i < (sys->natoms)-1; i+=size) {

      printf("Process %d out of %d: dealing with particle %d\n", rank, size , i);
      for(int j=i+1; j < (sys->natoms); ++j) {

            /* particles have no interactions with themselves */
            //if (i==j) continue; // to be activated if N's law is not exploited

            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
            ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
            rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
            r = sqrt(rx*rx + ry*ry + rz*rz);

            /* compute force and energy if within cutoff */
            if (r < sys->rcut) {
                ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r
                                         +6*pow(sys->sigma/r,6.0)/r);

                sys->epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
                                               -pow(sys->sigma/r,6.0));
		
#ifdef _MPI
		sys->cx[i] += rx/r*ffac;
                sys->cy[i] += ry/r*ffac;
		sys->cz[i] += rz/r*ffac;
                sys->cx[j] -= rx/r*ffac;
                sys->cy[j] -= ry/r*ffac;
                sys->cz[j] -= rz/r*ffac;
#else
                sys->fx[i] += rx/r*ffac;
                sys->fy[i] += ry/r*ffac;
                sys->fz[i] += rz/r*ffac;
                sys->fx[j] -= rx/r*ffac;
                sys->fy[j] -= ry/r*ffac;
		sys->fz[j] -= rz/r*ffac;
#endif


            } /*end of if r<rcut*/
        } /*end of for cycle on j*/
    } /*end of for cycle on i*/

#ifdef _MPI
    MPI_Reduce(sys->cx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
    MPI_Reduce(sys->cy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
    MPI_Reduce(sys->cz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
    double epot = sys->epot;
    MPI_Reduce(&epot, &sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm);
#endif
}
