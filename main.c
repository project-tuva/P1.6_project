/* 
 * simple lennard-jones potential MD code with velocity verlet.
 * units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <ljmd.h>
/*include mpi.h iff compiled with mpicc -D _MPI=1*/
#ifdef _MPI
#include <mpi.h>
#endif

/*debugging constants*/
#define D_MPI_INIT 1
/**/


/* MAIN */
int main(int argc, char **argv) 
{
#ifdef _MPI
  // INITIALIZE MPI ENVIRONMENT
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif /*_MPI*/

#if defined(_MPI) && defined(D_MPI_INIT)
  printf("\n------------------\n");
  printf("Hello from process %d out of %d\n", rank, size);
  printf("-------------------\n");
#endif /*defined(_MPI) && defined(D_MPI_INIT)*/
  
  int nprint, i;
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
    FILE *fp,*traj,*erg;
    mdsys_t sys;

    /* sets the system usyn parameters in stdin */
    set_mdsys(&sys,restfile,trajfile,ergfile,line,&nprint);

    /* allocate memory */
    allocate_mdsys(&sys);

    /* read restart - set initial conditions */
    set_ic(&sys,restfile);

    /* initialize forces and energies.*/
    sys.nfi=0;
    force(&sys);
    ekin(&sys);
    
    erg=fopen(ergfile,"w");
    traj=fopen(trajfile,"w");

    printf("Starting simulation with %d atoms for %d steps.\n",sys.natoms, sys.nsteps);
    printf("     NFI            TEMP            EKIN                 EPOT              ETOT\n");
    output(&sys, erg, traj);

    /**************************************************/
    /* main MD loop */
    for(sys.nfi=1; sys.nfi <= sys.nsteps; ++sys.nfi) {

        /* write output, if requested */
        if ((sys.nfi % nprint) == 0)
            output(&sys, erg, traj);

        /* propagate system and recompute energies */
        velverlet_1(&sys);
	force(&sys);
	velverlet_2(&sys);
        ekin(&sys);
    }
    /**************************************************/

    /* clean up: close files, free memory */
    printf("Simulation Done.\n");
    fclose(erg);
    fclose(traj);

    free_mdsys(&sys);
#ifdef _MPI    
    //MPI FINALIZE
    MPI_Finalize();
#endif /*_MPI*/

    return 0;
}
