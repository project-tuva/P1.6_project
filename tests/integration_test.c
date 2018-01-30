/* Integration Test
   Compute one full step integration for given forces and velocities
*/

// TEST CODE
// Comparison between files .rest and .out
#ifdef  _MPI
#include <mpi.h>
#endif

#include <ljmd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
  int rank=0;
  int size = 1;

#ifdef _MPI
  // INITIALIZE MPI ENVIRONMENT                                                                                  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif /*_MPI*/


  int nprint;
  char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
  mdsys_t sys;

  set_mdsys(&sys, restfile, trajfile, ergfile, line, &nprint);
  allocate_mdsys(&sys);
  if(rank==0){
  set_ic_f(&sys, restfile);
  
  // Apply Verlet Velocity algorithm
  velverlet_1(&sys);
  velverlet_2(&sys);

  // Write output to file
  test_output(&sys, trajfile);
  }
  // Deallocation
  free_mdsys(&sys);
  
#ifdef _MPI
  //MPI FINALIZE                                                                                                 
  MPI_Finalize();
#endif /*_MPI*/



  return 0;
} 
