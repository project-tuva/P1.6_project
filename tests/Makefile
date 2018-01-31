#Dirs
ROOT=..
EXEDIR=.
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include
LIBDIR_ser=$(ROOT)/Obj-serial
LIBDIR_omp=$(ROOT)/Obj-omp
LIBDIR_mpi=$(ROOT)/MPI

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -g -fopenmp -std=c99 -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR)
MPICC=mpicc
MPISIZE=2
LDLIBS=-lm  -g -p -L$(LIBDIR_ser) -L$(LIBDIR_omp) -Wl,-rpath,../Obj-serial -Wl,-rpath,../Obj-omp -L$(LIBDIR_mpi) -Wl,-rpath,../MPI

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/force_test_ser.x $(EXEDIR)/input_test.x $(EXEDIR)/kinetic_test_ser.x $(EXEDIR)/integration_test.x $(EXEDIR)/force_test_mpi.x $(EXEDIR)/input_test_mpi.x $(EXEDIR)/kinetic_test_mpi.x $(EXEDIR)/integration_test_mpi.x

clean:
	rm -f *.mod *.o *.out $(EXEDIR)/*.x

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h

# Serial executables
$(LIBDIR_ser)/libserial.so:
	$(MAKE) $(MFLAGS) -C ../Obj-serial

$(EXEDIR)/force_test_ser.x: $(LIBDIR_ser)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o force_test_ser.x force_test.c -lserial

$(EXEDIR)/kinetic_test_ser.x: $(LIBDIR_ser)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o kinetic_test_ser.x kinetic_test.c -lserial

$(EXEDIR)/input_test.x: $(LIBDIR_ser)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o input_test.x input_test.c -lserial

$(EXEDIR)/integration_test.x: $(LIBDIR_ser)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o integration_test.x integration_test.c -lserial


# OMP executables
$(LIBDIR_omp)/libljomp.so:
	$(MAKE) $(MFLAGS) -C ../Obj-omp

$(EXEDIR)/force_test_omp.x: $(LIBDIR_omp)/libljomp.so
	$(CC) $(LDLIBS) $(CFLAGS) -o force_test_omp.x force_test.c -lljomp

$(EXEDIR)/kinetic_test_omp.x: $(LIBDIR_omp)/libljomp.so
	$(CC) $(LDLIBS) $(CFLAGS) -o kinetic_test_omp.x kinetic_test.c -lljomp

# MPI executables
$(LIBDIR_mpi)/libljmd_MPI.so:
	$(MAKE) $(MFLAGS) -C ../MPI 

$(EXEDIR)/force_test_mpi.x: $(LIBDIR_mpi)/libljmd_MPI.so
	$(MPICC) $(LDLIBS) $(CFLAGS) -o force_test_mpi.x force_test.c -lljmd_MPI -D _MPI=1

$(EXEDIR)/input_test_mpi.x: $(LIBDIR_mpi)/libljmd_MPI.so
	$(MPICC) $(LDLIBS) $(CFLAGS) -o input_test_mpi.x input_test.c -lljmd_MPI -D _MPI=1

$(EXEDIR)/kinetic_test_mpi.x: $(LIBDIR_mpi)/libljmd_MPI.so
	$(MPICC) $(LDLIBS) $(CFLAGS) -o kinetic_test_mpi.x kinetic_test.c -lljmd_MPI -D _MPI=1

$(EXEDIR)/integration_test_mpi.x: $(LIBDIR_mpi)/libljmd_MPI.so
	$(MPICC) $(LDLIBS) $(CFLAGS) -o integration_test_mpi.x integration_test.c -lljmd_MPI -D _MPI=1



# dependencies
#ljmd.o: ljmd.c

check: input_test kinetic_test force_test integration_test

input_test: input_test.x input_test_mpi.x
	./input_test.x < input_test.inp
	diff -w input_test.inp input_test.out

	mpirun -np $(MPISIZE) ./input_test_mpi.x  < input_test.inp
	diff -w input_test.inp input_test.out

kinetic_test: kinetic_test_ser.x kinetic_test_mpi.x
	./kinetic_test_ser.x < kinetic1.test
	diff -w kinetic.rest kinetic1.out || exit 1
	./kinetic_test_ser.x < kinetic2.test
	diff -w kinetic.rest kinetic2.out || exit 1
	./kinetic_test_ser.x < kinetic3.test
	diff -w kinetic.rest kinetic3.out || exit 1

	mpirun -np $(MPISIZE) ./kinetic_test_mpi.x < kinetic1.test
	diff -w kinetic.rest kinetic1.out || exit 1
	mpirun -np $(MPISIZE) ./kinetic_test_mpi.x < kinetic2.test
	diff -w kinetic.rest kinetic2.out || exit 1
	mpirun -np $(MPISIZE) ./kinetic_test_mpi.x < kinetic3.test
	diff -w kinetic.rest kinetic3.out || exit 1

force_test: force_test_ser.x force_test_mpi.x
	./force_test_ser.x < force1.test
	diff -w force1.rest force1.out || exit 1
	./force_test_ser.x < force2.test
	diff -w force2.rest force2.out || exit 1
	./force_test_ser.x < force3.test
	diff -w force3.rest force3.out || exit 1

	mpirun -np $(MPISIZE) ./force_test_mpi.x < force1.test
	diff -w force1.rest force1.out || exit 1
	mpirun -np $(MPISIZE) ./force_test_mpi.x < force2.test
	diff -w force2.rest force2.out || exit 1
	mpirun -np $(MPISIZE) ./force_test_mpi.x < force3.test
	diff -w force3.rest force3.out || exit 1

integration_test: integration_test.x integration_test_mpi.x
	./integration_test.x < integration1.test
	diff -w integration1.rest integration1.out || exit 1

	mpirun -np $(MPISIZE) ./integration_test_mpi.x < integration1.test
	diff -w integration1.rest integration1.out || exit 1 
