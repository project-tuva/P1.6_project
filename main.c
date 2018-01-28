/* 
 * simple lennard-jones potential MD code with velocity verlet.
 * units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 */

/*include mpi.h iff compiled with mpicc -D _MPI=1*/
#ifdef _MPI
#include <mpi.h>
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <ljmd.h>

/*debugging constants MPI code*/
#define D_MPI_INIT 1
#define D_NSIZE 1
/**/


/* MAIN */
int main(int argc, char **argv) 
{
#ifdef _MPI
  // INITIALIZE MPI ENVIRONMENT
  int size=1;
  int rank=0;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif /*_MPI*/

#if defined(_MPI) && (D_MPI_INIT)
  printf("\n------------------\n");
  printf("Hello from process %d out of %d\n", rank, size);
  printf("-------------------\n");
  // MPI_Finalize();
  //return 0;
#endif /*defined(_MPI) && defined(D_MPI_INIT)*/

  
  int nprint;
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
    FILE *traj,*erg;
    mdsys_t sys;
    
    // PROBLEMA PERCHE SOLO PROCESSO 0 LEGGE STDIN E GLI ALTRI RESTANO A BOCCA ASCIUTTA!
    /* sets the system usyn parameters in stdin */
    set_mdsys(&sys,restfile,trajfile,ergfile,line,&nprint, rank, size);
    
#if defined ( _MPI) && defined(D_NSIZE) 
    /*util: set nsize=n atoms assigned to this process*/
    //set_nsize(&sys, rank, size);
    //    printf("this is rank %d with %d particles\n", rank, sys.nsize);
    printf("Process %d out of %d data: %d \t %d \t %.5f \t %.5f \t %.5f \t %.5f \t %.5f",\
	   rank, size,		\
	   sys.natoms, sys.nsize, sys.mass, sys.epsilon, sys.sigma, sys.rcut, sys.box);    
    MPI_Finalize();
    return 0;
#endif

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
