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


int main(void)
  {
    int nprint;
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
    mdsys_t sys;
     
    set_mdsys(&sys, restfile, trajfile, ergfile, line, &nprint);

    allocate_mdsys(&sys);
    
    set_ic_f(&sys, restfile);
      
    ekin(&sys);

    test_output(&sys, trajfile);

    free_mdsys(&sys);

    return 0;
  }

