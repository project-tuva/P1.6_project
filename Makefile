# -*- Makefile -*-
SHELL=/bin/bash
############################################
# derived makefile variables
OBJ_SERIAL=$(SRC:src/%.f90=Obj-serial/%.o)
############################################

default: serial omp

serial:
	$(MAKE) $(MFLAGS) -C Obj-serial

omp:
	$(MAKE) $(MFLAGS) -C Obj-omp

clean:
	$(MAKE) $(MFLAGS) -C Obj-serial clean
	$(MAKE) $(MFLAGS) -C Obj-omp clean
	$(MAKE) $(MFLAGS) -C examples clean
	$(MAKE) $(MFLAGS) -C tests clean

check: serial 
	$(MAKE) $(MFLAGS) -C examples check
