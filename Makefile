# -*- Makefile -*-
SHELL=/bin/bash
############################################
# derived makefile variables
#OBJ_SERIAL=$(SRC:src/%.f90=Obj-serial/%.o)
############################################

default: serial omp mpi

serial:
	$(MAKE) $(MFLAGS) -C Obj-serial

omp:
	$(MAKE) $(MFLAGS) -C Obj-omp

mpi:
	$(MAKE) $(MFLAGS) -C MPI

clean:
	$(MAKE) $(MFLAGS) -C Obj-serial clean
	$(MAKE) $(MFLAGS) -C Obj-omp clean
	$(MAKE) $(MFLAGS) -C examples clean
	$(MAKE) $(MFLAGS) -C tests clean
	$(MAKE) $(MFLAGS) -C MPI clean

check: serial mpi omp
	$(MAKE) $(MFLAGS) -C examples check