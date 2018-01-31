#Dirs
ROOT=..
EXEDIR=.
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include
LIBDIR_ser=$(ROOT)/Obj-serial
LIBDIR_omp=$(ROOT)/Obj-omp

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc

CFLAGS=-Wall -fopenmp -std=c99 -O3 -ffast-math -pg -g -I$(HEADDIR)
LDLIBS=-lm -pg -g -p -L$(LIBDIR_ser) -L$(LIBDIR_omp) -Wl,-rpath,../Obj-serial -Wl,-rpath,../Obj-omp

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/force_test_ser.x $(EXEDIR)/input_test.x $(EXEDIR)/kinetic_test_ser.x $(EXEDIR)/integration_test.x $(LIBDIR_ser)/libserial.so \
	$(EXEDIR)/force_test_omp.x $(EXEDIR)/input_test.x $(EXEDIR)/kinetic_test_omp.x $(EXEDIR)/integration_test.x $(LIBDIR_omp)/libljomp.so

clean:
	rm -f *.mod *.o *.out *~ $(EXEDIR)/force_test_ser.x $(EXEDIR)/input_test.x $(EXEDIR)/kinetic_test_ser.x $(EXEDIR)/integration_test.x \
	$(EXEDIR)/force_test_omp.x $(EXEDIR)/input_test.x $(EXEDIR)/kinetic_test_omp.x $(EXEDIR)/integration_test.x

$(OBJ): $(HEADDIR)/ljmd.h 

%.o: %.c
	$(CC) -c -fPIC $< $(CFLAGS) -o $@  

$(LIBDIR_ser)/libserial.so:
	$(MAKE) $(MFLAGS) -C ../Obj-serial

$(LIBDIR_omp)/libljomp.so:
	$(MAKE) $(MFLAGS) -C ../Obj-omp

# linker rule
force_test_ser.x: $(LIBDIR_ser)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o force_test_ser.x force_test.c -lserial

force_test_omp.x: $(LIBDIR_omp)/libljomp.so
	$(CC) $(LDLIBS) $(CFLAGS) -o force_test_omp.x force_test.c -lljomp

input_test.x: $(LIBDIR_ser)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o input_test.x input_test.c -lserial

kinetic_test_ser.x: $(LIBDIR_ser)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o kinetic_test_ser.x kinetic_test.c -lserial

kinetic_test_omp.x: $(LIBDIR_omp)/libljomp.so
	$(CC) $(LDLIBS) $(CFLAGS) -o kinetic_test_omp.x kinetic_test.c -lljomp

integration_test.x: $(LIBDIR_ser)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o integration_test.x integration_test.c -lserial

check: input_test kinetic_test force_test integration_test

input_test: input_test.x
	./input_test.x < input_test.inp
	diff -w input_test.inp input_test.out

kinetic_test: kinetic_test_ser.x kinetic_test_omp.x
	./kinetic_test_ser.x < kinetic1.test 
	diff -w kinetic.rest kinetic1.out || exit 1
	./kinetic_test_ser.x < kinetic2.test 
	diff -w kinetic.rest kinetic2.out || exit 1 
	./kinetic_test_ser.x < kinetic3.test 
	diff -w kinetic.rest kinetic3.out || exit 1

	./kinetic_test_omp.x < kinetic1.test 
	diff -w kinetic.rest kinetic1.out || exit 1
	./kinetic_test_omp.x < kinetic2.test 
	diff -w kinetic.rest kinetic2.out || exit 1 
	./kinetic_test_omp.x < kinetic3.test 
	diff -w kinetic.rest kinetic3.out || exit 1

force_test: force_test_ser.x force_test_omp.x
	./force_test_ser.x < force1.test 
	diff -w force1.rest force1.out || exit 1
	./force_test_ser.x < force2.test 
	diff -w force2.rest force2.out || exit 1 
	./force_test_ser.x < force3.test 
	diff -w force3.rest force3.out || exit 1

	./force_test_omp.x < force1.test 
	diff -w force1.rest force1.out || exit 1
	./force_test_omp.x < force2.test 
	diff -w force2.rest force2.out || exit 1 
	./force_test_omp.x < force3.test 
	diff -w force3.rest force3.out || exit 1

integration_test: integration_test.x
	./integration_test.x  < integration1.test || exit 1
	diff -w integration1.rest integration1.out || exit 1
	./integration_test.x  < integration2.test || exit 1
	diff -w integration2.rest integration2.out || exit 1
	./integration_test.x  < integration3.test || exit 1
	diff -w integration3.rest integration3.out || exit 1
	./integration_test.x  < integration4.test || exit 1
	diff -w integration4.rest integration4.out || exit 1

