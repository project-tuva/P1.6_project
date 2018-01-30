#Dirs
ROOT=..
EXEDIR=$(ROOT)
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include
LIBDIR=$(ROOT)/Obj-omp

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -g -fopenmp -std=c99 -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR)
LDLIBS=-lm -L$(LIBDIR) -Wl,-rpath,Obj-omp -Wl,-rpath,../Obj-omp

MAIN=$(ROOT)/main

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/ljmd-omp.x

clean:
	rm -f *.mod *.o $(EXEDIR)/ljmd-serial.x $(OBJ) $(EXEDIR)/*.o 

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h 

%.o: %.c
	$(CC) -c -fPIC $< $(CFLAGS) -o $@  

$(LIBDIR)/libljomp.so: $(OBJ)
	$(CC) -shared $(OBJ) -o $(LIBDIR)/libljomp.so

$(EXEDIR)/ljmd-omp.x: $(MAIN).c $(LIBDIR)/libljomp.so
	$(CC) $(LDLIBS) $(CFLAGS) -o $(EXEDIR)/ljmd-omp.x $(MAIN).c -lljomp
