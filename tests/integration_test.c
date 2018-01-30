/* Integration Test
   Compute one full step integration for given forces and velocities
*/

// TEST CODE
// Comparison between files .rest and .out
#include <ljmd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
  int nprint;
  char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
  mdsys_t sys;

  set_mdsys(&sys, restfile, trajfile, ergfile, line, &nprint);
  allocate_mdsys(&sys);

  set_ic_f(&sys, restfile);
  
  // Apply Verlet Velocity algorithm
  velverlet_1(&sys);
  velverlet_2(&sys);

  // Write output to file
  test_output(&sys, trajfile);
  
  // Deallocation
  free_mdsys(&sys);
  
  return 0;
}
