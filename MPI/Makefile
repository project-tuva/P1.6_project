#Dirs
ROOT=..
EXEDIR=$(ROOT)
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include

# -*- Makefile -*-
SHELL=/bin/bash
CC=mpicc
CFLAGS=-Wall -g -std=c99 -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR) -D _MPI=1
LDLIBS=-lm -lrt
VPATH=../src:..
#MAIN=$(ROOT)/main

SRC=force_compute.c input.c output.c utilities.c verlet_1.c verlet_2.c main.c
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/ljmd_MPI.x

clean:
	rm -f *.mod *.o $(EXEDIR)/ljmd_MPI.x $(OBJ) $(EXEDIR)/*.o 

# linker rule
$(EXEDIR)/ljmd_MPI.x: $(OBJ) 
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@  







.PHONY: clean
