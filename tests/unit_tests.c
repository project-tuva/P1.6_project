/*source file for tests*/
/*
  Let's consider this config (implemented in force1.rest):
  A(0.1,0,0)
  B(0.495,0,0)
  C(0.505,0,0)
  D(0.9,0,0)

  UNIT TEST - EXPECTED BEHAVIOR:
  force1.test: if r in [0, 0.05) no interactions
  force2.test: if r in [0.05, 0.1) B<->C
  force3.test: if r in [0.1, 0.395) B<->C & A<->D

*/

// TEST CODE


#include <ljmd.h>
#include <stdio.h>
#include <stdlib.h>


int main(void){
  
  // This test verify that input parameter data is read correctly.
  mdsys_t sys;
  char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
  int nprint;
  FILE *fp;
  fp=fopen("prova.txt", "w");

  set_mdsys(&sys,restfile,trajfile,ergfile,line,&nprint);

  fprintf(fp,
	  "%d\t", sys.natoms, "# natoms\n",
	  ".3%f\t", sys.mass, "# mass in AMU\n",
	  ".4%f\t", sys.epsilon, "# epsilon in kcal/mol\n",
	  ".3%f\t", sys.sigma, "# sigma in angstrom\n",
	  ".1%f\t", sys.rcut, "# rcut in angstrom\n",
	  ".4%f\t", sys.box, "# box length (in angstrom)\n",
	  "%s\t", restfile, "# restart\n",
	  "%s\t", trajfile, "# trajectory\n",
	  "%s\t", ergfile, "# energies\n",
	  "%d\t", nsteps, "# nr MD steps\n",
	  "%.1f\t", sys.dt, "# MD time step (in fs)\n",
	  "%d\t", nfi, "# output print frequency\n");
  
  fclose(fp);



  return 0;
}
