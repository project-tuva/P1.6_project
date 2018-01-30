#Dirs
ROOT=..
EXEDIR=.
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include
LIBDIR_mpi=$(ROOT)/MPI

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -g -std=c99 -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR)
MPICC=mpicc
MPISIZE=2
LDLIBS=-lm -pg -g -p -L$(LIBDIR_mpi) -Wl,-rpath,../MPI

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/force_test_ser.x $(EXEDIR)/input_test_ser.x $(EXEDIR)/kinetic_test_ser.x $(EXEDIR)/integration_test_ser.x $(EXEDIR)/force_test_mpi.x #$(EXEDIR)/input_test_mpi.x $(EXEDIR)/kinetic_test_mpi.x $(EXEDIR)/integration_test_mpi.x

clean:
	rm -f *.mod *.o *.out $(EXEDIR)/force_test_ser.x $(EXEDIR)/input_test_ser.x $(EXEDIR)/kinetic_test_ser.x $(EXEDIR)/integration_test_ser.x $(EXEDIR)/force_test_mpi.x #$(EXEDIR)/input_test_mpi.x $(EXEDIR)/kinetic_test_mpi.x $(EXEDIR)/integration_test_mpi.x

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h

%.o: %.c
	$(MPICC) -fPIC -c $< $(CFLAGS) -o $@  

$(LIBDIR_mpi)/libljmd_MPI.so:
	$(MAKE) $(MFLAGS) -C ../MPI

$(EXEDIR)/force_test_mpi.x: $(LIBDIR_mpi)/libljmd_MPI.so 
	$(MPICC) $(LDLIBS) $(CFLAGS) -o force_test_mpi.x force_test.c -lljmd_MPI 
#$(EXEDIR)/input_test_mpi.x
#	$(MPICC) $(LDLIBS) $(CFLAGS) -o input_test_mpi.x input_test.c -lljmd_MPI

#$(EXEDIR)/kinetic_test_mpi.x
#	$(MPICC) $(LDLIBS) $(CFLAGS) -o kinetic_test_mpi.o kinetic_test.c -lljmd_MPI

#$(EXEDIR)/integration_test_mpi.x
#	$(MPICC) $(LDLIBS) $(CFLAGS) -o integration_test_mpi.o integration_test.c -lljmd_MPI



# dependencies
#ljmd.o: ljmd.c 

check: input_test kinetic_test force_test integration_test

input_test: input_test_ser.x input_test_mpi.x
	./input_test_ser.x < input_test.inp
	diff -w input_test.inp input_test.out

	./input_test_mpi.x  < input_test.inp
	diff -w input_test.inp input_test.out

kinetic_test: kinetic_test_ser.x kinetic_test_mpi.x
	./kinetic_test_ser.x < kinetic1.test 
	diff -w kinetic.rest kinetic1.out || exit 1
	./kinetic_test_ser.x < kinetic2.test 
	diff -w kinetic.rest kinetic2.out || exit 1 
	./kinetic_test_ser.x < kinetic3.test 
	diff -w kinetic.rest kinetic3.out || exit 1

	./kinetic_test_mpi.x < kinetic1.test
	diff -w kinetic.rest kinetic1.out || exit 1
	./kinetic_test_mpi.x < kinetic2.test
	diff -w kinetic.rest kinetic2.out || exit 1
	./kinetic_test_mpi.x < kinetic2.test
	diff -w kinetic.rest kinetic3.out || exit 1

force_test: force_test_ser.x force_test_mpi.x
	./force_test_ser.x < force1.test 
	diff -w force1.rest force1.out || exit 1
	./force_test_ser.x < force2.test 
	diff -w force2.rest force2.out || exit 1 
	./force_test_ser.x < force3.test 
	diff -w force3.rest force3.out || exit 1

	./force_test_mpi.x < force1.test
	diff -w force1.rest force1.out || exit 1
	./force_test_mpi.x < force2.test
	diff -w force2.rest force2.out || exit 1
	./force_test_mpi.x < force3.test
	diff -w force3.rest force3.out || exit 1

#integration_test: integration_test_ser.x integration_test_mpi.x
#	./integration_test.x <  || exit 1 
