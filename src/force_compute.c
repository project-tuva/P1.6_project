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
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);


    double * bsend;
    //    double *recvb;
    /*evaluate buffer size*/
    int rem = sys->natoms % size;
    double dimbuff;
    dimbuff = 3* (sys->nsizes) * sizeof(double);
    if(rank>rem)
      dimbuff+=3;
    
    /*alloc memory for send and recv buffers*/
    bsend = (double *)malloc(dimbuff);
    azzero(sys->bsend, dimbuff);
    //    recvb = (double *)malloc(dimbuff);

    int off=0; // offset to fill in bsend
    /*each process computes the force for natoms/size(+1 if neeed) particles*/
    for(int i=rank; i < (sys->natoms)-1; i+=size) {
        for(int j=i+1; j < (sys->natoms); ++j) {

            /* particles have no interactions with themselves */
            //if (i==j) continue; // managed by altering the cycling rules

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

            } /*end of if r<rcut*/
        } /*end of for cycle on j*/

    } /*end of for cycle on i*/



    /*Broadcast computed values to other processes*/
    for(int R=0; R<size; ++R){
      /*fill in bsend*/
	for(int i=rank; i < sys->natoms; ++i){
	  bsend[off*3] = sys->fx[i];
	  bsend[off*3+1] = sys->fy[i];
	  bsend[off*3+2] = sys->fz[i];
	  ++off;
	} /*end of for loop on i to fill in bsend*/
	
	/*Broadcast data from current rank to all other ranks*/
	MPI_Bcast(bsend, dimbuff, MPI_DOUBLE, R, sys->mpicomm);

	/*Copy buffer elements into the right memory cell in fx fy and fz*/
	

    } /*end of for loop on R (ranks)*/


    free(bsend);
    bsend=NULL;
    free(brecv);
    brecv=NULL;


}
