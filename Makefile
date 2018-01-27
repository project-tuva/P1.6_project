# -*- Makefile -*-
SHELL=/bin/bash
############################################
# derived makefile variables
#OBJ_SERIAL=$(SRC:src/%.f90=Obj-serial/%.o)
############################################

default: serial

serial:
	$(MAKE) $(MFLAGS) -C Obj-$@

mpi:
	$(MAKE) $(MFLAGS) -C MPI

clean:
	$(MAKE) $(MFLAGS) -C Obj-serial clean
	$(MAKE) $(MFLAGS) -C examples clean
	$(MAKE) $(MFLAGS) -C tests clean

check: serial 
	$(MAKE) $(MFLAGS) -C examples check
