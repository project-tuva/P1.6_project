/* Integration Test
Compute one full step integration for given forces and velocities
*/

// TEST CODE

#include <ljmd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TOLERANCE 1e-06

int main(){
  mdsys_t sys;
  FILE *fp;
  time_t t;

  // Set the seed for random generator
  srand((unsigned) time(&t));
  
  // Initialize sys
  sys.natoms = 3;
  sys.mass = 39.948;
  sys.dt = 5.0;
  sys.epsilon = 0.2379;
  sys.sigma = 3.405;
  sys.box = 17.1580;
  sys.rcut = 8.5;
  
  // Allocate the structure
  allocate_mdsys(&sys); 

  // Initial positions
  double posix[sys.natoms];
  double posiy[sys.natoms];
  double posiz[sys.natoms];

  
  // Test 1: still particles
  for (int i=0; i<sys.natoms; ++i){
    sys.rx[i]= ((double)rand()/(RAND_MAX)*sys.box); 
    sys.ry[i]= ((double)rand()/(RAND_MAX)*sys.box);
    sys.rz[i]= ((double)rand()/(RAND_MAX)*sys.box);
    posix[i] =sys.rx[i];
    posiy[i] =sys.ry[i];
    posiz[i] =sys.rz[i];
    /*
    printf("%f %f\n", sys.rx[i], posix[i]);
    printf("%f %f\n", sys.ry[i], posiy[i]);
    printf("%f %f\n", sys.rz[i], posiz[i]);
    */
    sys.vx[i]=0.0;
    sys.vy[i]=0.0;
    sys.vz[i]=0.0;
    
    sys.fx[i]=0.0;
    sys.fy[i]=0.0;
    sys.fz[i]=0.0;
    
  }

  // Apply Verlet Velocity algorithm
  velverlet_1(&sys);
  velverlet_2(&sys);

  // Check positions
 
  fp = fopen("integration_test.out","w");
  fprintf(fp, "TEST 1: STILL PARTICLE\n");
  
    for (int i=0; i<sys.natoms; ++i){
      if(fabs(posix[i]-sys.rx[i])>TOLERANCE){
	fprintf(fp, "Test FAILED. Position rx for particle %d is not still.\n",
		i);
		return 1;
      }
     
      else if(fabs(posiy[i]-sys.ry[i])>TOLERANCE){
	fprintf(fp, "Test FAILED. Position ry for particle %d is not still.\n",
		i);
		return 1;
      }
      else if(fabs(posiz[i]-sys.rz[i])>TOLERANCE){
	fprintf(fp, "Test FAILED. Position rz for particle %d is not still.\n",
		i);
	return 1;
	}
	else{
	  fprintf(fp, "Test PASSED. Particle %d is not moving\n", i);
	}
    }
    fclose(fp);
  
  // Deallocation
  free_mdsys(&sys);
  
  return 0;
}
