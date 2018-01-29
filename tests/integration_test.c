/* Integration Test
   Compute one full step integration for given forces and velocities
*/

// TEST CODE

#include <ljmd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TOLERANCE 1e-03
// 1A = 10^-10 m

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

  //   set_mdsys(&sys,restfile, trajfile, ergfile, line, &nprint);

  // Allocate the structure
  allocate_mdsys(&sys); 

  // Initial positions
  double posix[sys.natoms];
  double posiy[sys.natoms];
  double posiz[sys.natoms];

  // -----------------------------------------------------------------------
  // TEST 1: still particles
  // -----------------------------------------------------------------------
  // Initialization of positions, velocities and forces
  for (int i=0; i<sys.natoms; ++i){
    sys.rx[i] = ((double)rand()/(RAND_MAX)*sys.box); 
    sys.ry[i] = ((double)rand()/(RAND_MAX)*sys.box);
    sys.rz[i] = ((double)rand()/(RAND_MAX)*sys.box);
    posix[i] = sys.rx[i];
    posiy[i] = sys.ry[i];
    posiz[i] = sys.rz[i];
    
    sys.vx[i] = 0.0;
    sys.vy[i] = 0.0;
    sys.vz[i] = 0.0;
    
    sys.fx[i] = 0.0;
    sys.fy[i] = 0.0;
    sys.fz[i] = 0.0;   
  }

  // Apply Verlet Velocity algorithm
  velverlet_1(&sys);
  velverlet_2(&sys);

  // Check positions
  fp = fopen("integration_test.out","w");
  fprintf(fp, "TEST 1: STILL PARTICLES\n");
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

  // -----------------------------------------------------------------------
  // TEST 2: moving particles with unitary velocity
  // -----------------------------------------------------------------------
  // Initialization of positions, velocities and forces
  // Particle 0
  sys.vx[0] = 1;
  sys.vy[0] = 0;
  sys.vz[0] = 0;
   
  // Particle 1
  sys.vx[1] = 0;
  sys.vy[1] = 1;
  sys.vz[1] = 0;
    
  // Particle 2
  sys.vx[2] = 0;
  sys.vy[2] = 0;
  sys.vz[2] = 1;
    
  for (int i=0; i<sys.natoms; ++i){
    sys.rx[i] = 0;
    sys.ry[i] = 0;
    sys.rz[i] = 0;
    
    sys.fx[i] = 0.0;
    sys.fy[i] = 0.0;
    sys.fz[i] = 0.0;
  }
  
  // Apply Verlet Velocity algorithm
  velverlet_1(&sys);
  velverlet_2(&sys);

  double finalx[sys.natoms];
  double finaly[sys.natoms];
  double finalz[sys.natoms];

  azzero(finalx, sys.natoms);
  azzero(finaly, sys.natoms);
  azzero(finalz, sys.natoms);

  finalx[0] = sys.dt;
  finaly[1] = sys.dt;
  finalz[2] = sys.dt;
  
  // Check position
  fp = fopen("integration_test.out", "a");
  fprintf(fp, "\nTEST 2: MOVING PARTICLES WITH UNITARY VELOCITY\n");
  for (int i=0; i<sys.natoms; ++i){
    if(fabs(finalx[i]-sys.rx[i])>TOLERANCE){
      fprintf(fp, "Test FAILED. Position rx for particle %d is not correct\n",
	      i);
      return 1;
    }   
    else if(fabs(finaly[i]-sys.ry[i])>TOLERANCE){
      fprintf(fp, "Test FAILED. Position ry for particle %d is not correct\n",
	      i);
      return 1;
    }
    else if(fabs(finalz[i]-sys.rz[i])>TOLERANCE){
      fprintf(fp, "Test FAILED. Position rz for particle %d is not correct\n",
	      i);
      fprintf(fp, "%f %f\n", sys.dt, sys.rz[i]);

      return 1;
    }
    else{
      fprintf(fp, "Test PASSED. The position for particle %d is correct\n", i);
    }
  }
  fclose(fp);

  // -----------------------------------------------------------------------
  // TEST 3: moving particles with unitary force
  // -----------------------------------------------------------------------
  // Initialization of positions, velocities and forces
  // Particle 0
  sys.fx[0] = 1;
  sys.fy[0] = 0;
  sys.fz[0] = 0;
   
  // Particle 1
  sys.fx[1] = 0;
  sys.fy[1] = 1;
  sys.fz[1] = 0;
    
  // Particle 2
  sys.fx[2] = 0;
  sys.fy[2] = 0;
  sys.fz[2] = 1;

  for (int i=0; i<sys.natoms; ++i){
    sys.rx[i] = 0;
    sys.ry[i] = 0;
    sys.rz[i] = 0;
  }

  azzero(sys.vx, sys.natoms);
  azzero(sys.vy, sys.natoms);
  azzero(sys.vz, sys.natoms);
  
  // Apply Verlet Velocity algorithm
  velverlet_1(&sys);
  velverlet_2(&sys);

  azzero(finalx, sys.natoms);
  azzero(finaly, sys.natoms);
  azzero(finalz, sys.natoms);
  
  for (int i=0; i<sys.natoms; i++){
    finalx[i] = sys.dt * sys.dt * 0.5 * sys.fx[i] / (mvsq2e*sys.mass);
    finaly[i] = sys.dt * sys.dt * 0.5 * sys.fy[i] / (mvsq2e*sys.mass);
    finalz[i] = sys.dt * sys.dt * 0.5 * sys.fz[i] / (mvsq2e*sys.mass);
  }
  
  // Check position
  fp = fopen("integration_test.out", "a");
  fprintf(fp, "\nTEST 3: MOVING PARTICLES WITH UNITARY FORCE\n");
  for (int i=0; i<sys.natoms; ++i){
    if(fabs(finalx[i]-sys.rx[i])>TOLERANCE){
      fprintf(fp, "Test FAILED. Position rx for particle %d is not correct\n",
	      i);
      fprintf(fp, "%f %f\n", finalx[i], sys.rx[i]);
      return 1;
    }   
    else if(fabs(finaly[i]-sys.ry[i])>TOLERANCE){
      fprintf(fp, "Test FAILED. Position ry for particle %d is not correct\n",
	      i);
      fprintf(fp, "%f %f\n", finaly[i], sys.ry[i]);
      return 1;
    }
    else if(fabs(finalz[i]-sys.rz[i])>TOLERANCE){
      fprintf(fp, "Test FAILED. Position rz for particle %d is not correct\n",
	      i);
      fprintf(fp, "%f %f\n", finalz[i], sys.rz[i]);
      return 1;
    }
    else{
      fprintf(fp, "Test PASSED. The position for particle %d is correct\n", i);
    }
  }
  fclose(fp);

  // -----------------------------------------------------------------------
  // TEST 4: moving particles with unitary velocity and force
  // -----------------------------------------------------------------------
  // Particle 0
  sys.vx[0] = 1;
  sys.vy[0] = 0;
  sys.vz[0] = 0;

  sys.fx[0] = 1;
  sys.fy[0] = 0;
  sys.fz[0] = 0;

  // Particle 1
  sys.vx[1] = 0;
  sys.vy[1] = 1;
  sys.vz[1] = 0;
  
  sys.fx[1] = 0;
  sys.fy[1] = 1;
  sys.fz[1] = 0;
    
  // Particle 2
  sys.vx[2] = 0;
  sys.vy[2] = 0;
  sys.vz[2] = 1;

  sys.fx[2] = 0;
  sys.fy[2] = 0;
  sys.fz[2] = 1;

  for (int i=0; i<sys.natoms; ++i){
    sys.rx[i] = 0;
    sys.ry[i] = 0;
    sys.rz[i] = 0;
  }
  
  // Apply Verlet Velocity algorithm
  velverlet_1(&sys);
  velverlet_2(&sys);
  
  azzero(finalx, sys.natoms);
  azzero(finaly, sys.natoms);
  azzero(finalz, sys.natoms);

  for (int i=0; i<sys.natoms; i++){
    finalx[i] =  sys.dt * sys.vx[i] + sys.dt * sys.dt * 0.5 * sys.fx[i] / (mvsq2e*sys.mass);
    finaly[i] =  sys.dt * sys.vy[i] + sys.dt * sys.dt * 0.5 * sys.fy[i] / (mvsq2e*sys.mass);
    finalz[i] =  sys.dt * sys.vz[i] + sys.dt * sys.dt * 0.5 * sys.fz[i] / (mvsq2e*sys.mass);
  }

  // Check position
  fp = fopen("integration_test.out", "a");
  fprintf(fp, "\nTEST 4: MOVING PARTICLES WITH UNITARY VELOCITY AND FORCE\n");
  for (int i=0; i<sys.natoms; ++i){
    if(fabs(finalx[i]-sys.rx[i])>TOLERANCE){
      fprintf(fp, "Test FAILED. Position rx for particle %d is not correct\n",
	      i);
      fprintf(fp, "%f %f\n", finalx[i], sys.rx[i]);
      return 1;
    }   
    else if(fabs(finaly[i]-sys.ry[i])>TOLERANCE){
      fprintf(fp, "Test FAILED. Position ry for particle %d is not correct\n",
	      i);
      fprintf(fp, "%f %f\n", finaly[i], sys.ry[i]);
      return 1;
    }
    else if(fabs(finalz[i]-sys.rz[i])>TOLERANCE){
      fprintf(fp, "Test FAILED. Position rz for particle %d is not correct\n",
	      i);
      fprintf(fp, "%f %f\n", finalz[i], sys.rz[i]);
      return 1;
    }
    else {
      fprintf(fp, "Test PASSED. The position for particle %d is correct\n", i);
    }
  }
  fclose(fp); 
  
  // Deallocation
  free_mdsys(&sys);
  
  return 0;
}
