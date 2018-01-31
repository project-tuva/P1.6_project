/*input.c: input methods*/
#include<ljmd.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#ifdef _MPI
#include <mpi.h>
#endif /*defined _MPI*/

int get_a_line(FILE *fp, char *buf)
{
    char tmp[BLEN], *ptr;

    /* read a line and cut of comments and blanks */
    if (fgets(tmp,BLEN,fp)) {
        int i;

        ptr=strchr(tmp,'#');
        if (ptr) *ptr= '\0';
        i=strlen(tmp); --i;
        while(isspace(tmp[i])) {
            tmp[i]='\0';
            --i;
        }
        ptr=tmp;
        while(isspace(*ptr)) {++ptr;}
        i=strlen(ptr);
        strcpy(buf,tmp);
        return 0;
    } else {
        perror("problem reading input");
        return -1;
    }
    return 0;
}



int set_nsize(int natoms, int rank, int size){
  int nsize;
  nsize = natoms / size;
  if(rank < natoms % size) /*manage remainders*/
    ++nsize;
  return nsize;
}




int set_mdsys(mdsys_t *sys,char restfile[BLEN],char trajfile[BLEN],char ergfile[BLEN],char line[BLEN],int *nprint)
{
  int rank =0;
  int size = 1;
#ifdef _MPI
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  sys->mpicomm = MPI_COMM_WORLD;
#endif /*defined MPI*/

  if(rank==0){
  /* read input file */
  if(get_a_line(stdin,line)) return 1;
  sys->natoms=atoi(line); 
  if(get_a_line(stdin,line)) return 1;
  sys->mass=atof(line); 
  if(get_a_line(stdin,line)) return 1;
  sys->epsilon=atof(line);
  if(get_a_line(stdin,line)) return 1;
  sys->sigma=atof(line);
  if(get_a_line(stdin,line)) return 1;
  sys->rcut=atof(line);
  if(get_a_line(stdin,line)) return 1;
  sys->box=atof(line);
  if(get_a_line(stdin,restfile)) return 1;
  if(get_a_line(stdin,trajfile)) return 1;
  if(get_a_line(stdin,ergfile)) return 1;
  if(get_a_line(stdin,line)) return 1;
  sys->nsteps=atoi(line);
  if(get_a_line(stdin,line)) return 1;
  sys->dt=atof(line);
  if(get_a_line(stdin,line)) return 1;
  *nprint=atoi(line);
}


#ifdef _MPI
  MPI_Bcast(&sys->natoms, 1, MPI_INT, 0, sys->mpicomm);
  MPI_Bcast(&sys->mass, 1, MPI_DOUBLE, 0, sys->mpicomm);
  MPI_Bcast(&sys->epsilon, 1, MPI_DOUBLE, 0, sys->mpicomm);
  MPI_Bcast(&sys->sigma, 1, MPI_DOUBLE, 0, sys->mpicomm);
  MPI_Bcast(&sys->rcut, 1, MPI_DOUBLE, 0, sys->mpicomm);
  MPI_Bcast(&sys->box, 1, MPI_DOUBLE, 0, sys->mpicomm);
  MPI_Bcast(restfile, BLEN, MPI_CHAR, 0, sys->mpicomm);
  MPI_Bcast(trajfile, BLEN, MPI_CHAR, 0, sys->mpicomm);
  MPI_Bcast(ergfile, BLEN, MPI_CHAR, 0, sys->mpicomm);
  MPI_Bcast(&sys->nsteps, 1, MPI_INT, 0, sys->mpicomm);
  MPI_Bcast(&sys->dt, 1, MPI_DOUBLE, 0, sys->mpicomm);
  MPI_Bcast(nprint, 1, MPI_INT, 0, sys->mpicomm);

#endif /*defined _MPI*/

  sys->nsize = set_nsize(sys->natoms,rank,size);

  return 0;

}

/* sets initial condition file named by the string restfile */
int set_ic(mdsys_t *sys, char restfile[BLEN]){

  FILE *fp;
  int i,foo;
  
  fp=fopen(restfile,"r");
  if(fp){
    for (i=0; i<sys->natoms; ++i) {
      foo=fscanf(fp,"%lf%lf%lf",sys->rx+i, sys->ry+i, sys->rz+i);
    }
    for (i=0; i<sys->natoms; ++i) {
      foo=fscanf(fp,"%lf%lf%lf",sys->vx+i, sys->vy+i, sys->vz+i);
    }
    fclose(fp);

} else {
    perror("cannot read restart file");
    return 3;
  }

  ++foo;

  return 0;  
  
}

/* sets initial condition file for kinetic energy test named by the string restfile */
int set_ic_f(mdsys_t *sys, char restfile[BLEN]){

  FILE *fp;
  int i,foo;
  
  fp=fopen(restfile,"r");
  if(fp){
    for (i=0; i<sys->natoms; ++i) {
      foo=fscanf(fp,"%lf%lf%lf",sys->rx+i, sys->ry+i, sys->rz+i);
    }
    for (i=0; i<sys->natoms; ++i) {
      foo=fscanf(fp,"%lf%lf%lf",sys->vx+i, sys->vy+i, sys->vz+i);
    }
    for (i=0; i<sys->natoms; ++i) {
      foo=fscanf(fp,"%lf%lf%lf",sys->fx+i, sys->fy+i, sys->fz+i);
    }
    fclose(fp);
  }  
  else {
    perror("cannot read restart file");
    return 3;
  }

  ++foo;

  return 0;  
  
}
