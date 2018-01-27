#Dirs
ROOT=..
EXEDIR=$(ROOT)
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include

# -*- Makefile -*-
SHELL=/bin/bash
CC=mpicc
CFLAGS=-Wall -g -std=c99 -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR)
LDLIBS=-lm -lrt

MAIN=$(ROOT)/ljmd_MPI

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/ljmd_MPI.x
#default:
#	echo $(OBJ)
#	echo $(SRC)


clean:
	rm -f *.mod *.o $(EXEDIR)/ljmd_MPI.x $(OBJ) $(EXEDIR)/*.o 

# linker rule
$(EXEDIR)/ljmd_MPI.x: $(OBJ) $(MAIN).o
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@  

$(MAIN).o: $(MAIN).c
	$(CC) -c $< $(CFLAGS) -o $(MAIN).o

# dependencies
#ljmd.o: ljmd.c