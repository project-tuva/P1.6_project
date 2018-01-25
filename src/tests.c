/*tests.c: source file where the test functions are implemented*/
#include <ljmd.h>
#include <stdio.h>

struct _mdsys {
    int natoms,nfi,nsteps;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
};
typedef struct _mdsys mdsys_t;
//input test
void test2(){

}


int main(int argc, char **argv)
{
  double k;
  force();
  printf();

  verlet_1();
  verlet_2();
  printf("time of full step integration is done!");
  
  k = ekin();
  printf("kinetic energy is %f", k);
  
  if()
    printf("input parameter data is read correctly");
    return 0;
    
    return 1;
}
