/*output.c: output methods*/
#include<ljmd.h>
#include <stdio.h>

void output(mdsys_t *sys, FILE *erg, FILE *traj)
{
  int i;

  printf("% 8d % 20.8f % 20.8f % 20.8f % 20.8f\n", sys->nfi, sys->temp, sys->ekin, sys->epot, sys->ekin+sys->epot);
  fprintf(erg,"% 8d % 20.8f % 20.8f % 20.8f % 20.8f\n", sys->nfi, sys->temp, sys->ekin, sys->epot, sys->ekin+sys->epot);
  fprintf(traj,"%d\n nfi=%d etot=%20.8f\n", sys->natoms, sys->nfi, sys->ekin+sys->epot);
  for (i=0; i<sys->natoms; ++i) {
    fprintf(traj, "Ar  %20.8f %20.8f %20.8f\n", sys->rx[i], sys->ry[i], sys->rz[i]);
  }
}



void test_output(mdsys_t *sys, char *trajname)
{
  int i;
  FILE * traj;
  traj = fopen(trajname, "w");
  
  printf( "Starting force test with %d atoms for %d steps.\n",sys->natoms, sys->nsteps);

  for (i=0; i<sys->natoms; ++i) {
    fprintf(traj, "%.5f %.5f %.5f\n", sys->rx[i], sys->ry[i], sys->rz[i]);
  }

  for (i=0; i<sys->natoms; ++i) {
    fprintf(traj, "%.5f %.5f %.5f\n", sys->vx[i], sys->vy[i], sys->vz[i]);
  }

  for (i=0; i<sys->natoms; ++i) {
    fprintf(traj, "%.5f %.5f %.5f\n", sys->fx[i], sys->fy[i], sys->fz[i]);
  }
  
  fclose(traj);
}


