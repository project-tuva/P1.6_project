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

int main(){

  // This test verify that input parameter data is read correctly.
  // compare to input with diff -w fil1 file2
   
  printf("INPUT PARAMETER TEST: started\n");
  mdsys_t sys;
  char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
  int nprint;
  FILE *fp;
  fp=fopen("input_test.out", "w");

  set_mdsys(&sys,restfile,trajfile,ergfile,line,&nprint);

  fprintf(fp,"%d\t", sys.natoms);
  fprintf(fp,"# natoms\n");
  fprintf(fp,"%.3f\t", sys.mass);
  fprintf(fp,"# mass in AMU\n");
  fprintf(fp,"%.4f\t", sys.epsilon);
  fprintf(fp,"# epsilon in kcal/mol\n");
  fprintf(fp,"%.3f\t", sys.sigma);
  fprintf(fp,"# sigma in angstrom\n");
  fprintf(fp,"%.1f\t", sys.rcut);
  fprintf(fp,"# rcut in angstrom\n");
  fprintf(fp,"%.4f\t", sys.box);
  fprintf(fp,"# box length (in angstrom)\n");
  fprintf(fp,"%s\t", restfile);
  fprintf(fp,"# restart\n");
  fprintf(fp,"%s\t", trajfile);
  fprintf(fp,"# trajectory\n");
  fprintf(fp,"%s\t", ergfile);
  fprintf(fp,"# energies\n");
  fprintf(fp,"%d\t", sys.nsteps);
  fprintf(fp,"# nr MD steps\n");
  fprintf(fp,"%.1f\t", sys.dt);
  fprintf(fp,"# MD time step (in fs)\n");
  fprintf(fp,"%d\t", nprint);
  fprintf(fp,"# output print frequency\n");
  
  fclose(fp);

  printf("INPUT PARAMETER TEST: ended\n"); 
     
  return 0;
  
}
