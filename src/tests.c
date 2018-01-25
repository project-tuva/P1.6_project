/*tests.c: source file where the test functions are implemented
set up some simple unit tests, for istance:
-write C code that fills data structures, 
-calls the respective functions and outputs the changed data
-compare that against reference).
For example:
a) compute force for a few 2- or 3 particle systems with atoms 
   inside/outside the cutoff
b) compute one full step time integration for given forces and velocities 
  (no call to force())
c) compute kinetic energy for given velocities and mass
d) verify that input parameter data is read correctly.

Add these tests to the existing testing facility with Travis-CI.
*/
#include <stdio.h>

int main(){
  // dichiaro una sys con 4 stringhe di char di lunghezza BLEn e un integre
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
