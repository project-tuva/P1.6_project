/*input.c: input methods*/
#include<ljmd.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

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

int set_mdsys(mdsys_t *sys,char restfile[BLEN],char trajfile[BLEN],char ergfile[BLEN],char line[BLEN],int *nprint){

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
    azzero(sys->fx, sys->natoms);
    azzero(sys->fy, sys->natoms);
    azzero(sys->fz, sys->natoms);
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
