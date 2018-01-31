/*  
 * simple lennard-jones potential MD code with velocity verlet.
 * units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 */

/*include mpi.h iff compiled with mpicc -D _MPI=1*/
#ifdef  _MPI
#include <mpi.h>
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <ljmd.h>


/* MAIN */
int main(int argc, char **argv) 
{

  //  int size=1;
  int rank=0;

#ifdef _MPI
  // INITIALIZE MPI ENVIRONMENT                                                 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif /*_MPI*/

  double t_start = cclock();

  int nprint;
  char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
  FILE *traj,*erg;
  mdsys_t sys;
 
    /* sets the system usyn parameters in stdin */
  set_mdsys(&sys,restfile,trajfile,ergfile,line,&nprint);
  

    /* allocate memory */
  allocate_mdsys(&sys);


    /* read restart - set initial conditions */
  if(rank==0){
    set_ic(&sys,restfile);
   }




    /* initialize forces and energies.*/
    sys.nfi=0;

    force(&sys);

    if(rank==0){
      ekin(&sys);
    
      erg=fopen(ergfile,"w");
      traj=fopen(trajfile,"w");

      printf("Starting simulation with %d atoms for %d steps.\n",sys.natoms, sys.nsteps);
      fprintf(erg,"     NFI            TEMP            EKIN                 EPOT              ETOT\n");
      output(&sys, erg, traj);
      fclose(erg); //  only process 0                             
      fclose(traj); //  only process 0                                                                         
      

}
    
    /**************************************************/
    /* main MD loop */
    for(sys.nfi=1; sys.nfi <= sys.nsteps; ++sys.nfi) {
      /* write output, if requested */
      if (rank==0){
	if((sys.nfi % nprint) == 0){
	  erg=fopen(ergfile,"a");
	  traj=fopen(trajfile,"a");            
	  output(&sys, erg, traj);
          fclose(erg); //  only process 0
	  fclose(traj); //  only process 0
	}
        /* propagate system and recompute energies: verlet1 only by process 0 */
	    velverlet_1(&sys);
      }
        /* propagate system and recompute energies: forces: all processes */

	force(&sys);

	if (rank==0){
	  velverlet_2(&sys); // only process 0
	  ekin(&sys);// only process 0
	}
    }


    free_mdsys(&sys); // all processes

    double t_end = cclock();
    if(rank==0){
      FILE * time;
      time=fopen("timefile.txt","w+");
      fprintf(time, "Execution times: %.3f s\n", t_end-t_start);
      fclose(time);
      printf("Simulation Done.\n"); //  only process 0    
    }



#ifdef _MPI    
    //MPI FINALIZE
    MPI_Finalize();
#endif /*_MPI*/

    return 0;    

}
