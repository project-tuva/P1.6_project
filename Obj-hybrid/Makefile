#Dirs
ROOT=..
EXEDIR=$(ROOT)/
SRCDIR=$(ROOT)/src/
HEADDIR=$(ROOT)/include/
LIBDIR=$(ROOT)/Obj-hybrid/
OBJDIR=../Obj-hybrid/

# -*- Makefile -*-
SHELL=/bin/sh
CC=mpicc
CFLAGS=-Wall -fopenmp -O3 -ffast-math -pg -g -I$(HEADDIR) -std=gnu99 -D _MPI=1
LDLIBS=-lm -pg -g -L$(LIBDIR) -Wl,-rpath,Obj-hybrid -Wl,-rpath,../Obj-hybrid


MAIN=$(ROOT)/main

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)

TARGET=$(EXEDIR)ljmd-hybrid.x

.PHONY: clean

default: $(TARGET)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) -c -fPIC $(CFLAGS) $<

clean:
	rm -f *.mod *.o $(EXEDIR)ljmd-hybrid.x $(EXEDIR)*.o $(LIBDIR)libhybrid.so $(SRCDIR)*.o

$(LIBDIR)libhybrid.so: $(OBJ)
	$(CC) -shared $(OBJ) -o $(LIBDIR)libhybrid.so

$(TARGET): $(MAIN).c $(LIBDIR)libhybrid.so
	$(CC) $(LDLIBS) $(CFLAGS) -o $(EXEDIR)ljmd-hybrid.x $(MAIN).c -lhybrid
