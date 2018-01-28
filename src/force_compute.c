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
    int i,j;

    /* zero energy and forces */
    sys->epot=0.0;
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);

    for(i=rank; i < (sys->natoms)-1; i+=size) {
        for(j=i+1; j < (sys->natoms); ++j) {

            /* particles have no interactions with themselves */
            //if (i==j) continue;

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

                sys->fx[i] += rx/r*ffac;
                sys->fy[i] += ry/r*ffac;
                sys->fz[i] += rz/r*ffac;
            }
        }
    }


    double * buf = (double*)malloc( 3*((sys->natoms) -2)*sizeof(double)); // fx fy fz
    for(int i=0;i<size;++i){
      
      MPI_Bcast(buf, (sys->natoms), MPI_DOUBLE, i, sys->mpicomm);      
      
    }

    free(buf);
    buf=NULL;
}
