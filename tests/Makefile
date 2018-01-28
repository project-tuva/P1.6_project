#Dirs
ROOT=..
EXEDIR=.
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -fno-pie -I$(HEADDIR)
#CFLAGS=-Wall -std=c99 -O3 -ffast-math -fexpensive-optimizations -msse3 -pg -g -fno-pie -I$(HEADDIR)
#CFLAGS=-Wall -std=c99 -O3 -ffast-math  -pg -g -fno-pie -I$(HEADDIR)
LDLIBS=-lm -pg -fno-pie
#CFLAGS=-Wall -g -std=c99 -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR)
#LDLIBS=-lm

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/force_test.x $(EXEDIR)/input_test.x $(EXEDIR)/kinetic_test.x $(EXEDIR)/integration_test.x 

clean:
	rm -f *.mod *.o *.out $(EXEDIR)/force_test.x $(EXEDIR)/input_test.x $(EXEDIR)/kinetic_test.x $(EXEDIR)/integration_test.x 

# linker rule
force_test.x: $(OBJ) force_test.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

input_test.x: $(OBJ) input_test.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

kinetic_test.x: $(OBJ) kinetic_test.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

integration_test.x: $(OBJ) integration_test.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@  

force_test.o: force_test.c
	$(CC) -c $< $(CFLAGS) -o force_test.o

input_test.o: input_test.c
	$(CC) -c $< $(CFLAGS) -o input_test.o

kinetic_test.o: kinetic_test.c
	$(CC) -c $< $(CFLAGS) -o kinetic_test.o

integration_test.o: integration_test.c
	$(CC) -c $< $(CFLAGS) -o integration_test.o

# dependencies
#ljmd.o: ljmd.c 

check: input_test kinetic_test force_test integration_test

input_test: input_test.x
	./input_test.x < input_test.inp
	diff -w input_test.inp input_test.out

kinetic_test: kinetic_test.x
	./kinetic_test.x < kinetic1.test 
	diff -w kinetic.rest kinetic1.out || exit 1
	./kinetic_test.x < kinetic2.test 
	diff -w kinetic.rest kinetic2.out || exit 1 
	./kinetic_test.x < kinetic3.test 
	diff -w kinetic.rest kinetic3.out || exit 1

force_test: force_test.x
	./force_test.x < force1.test 
	diff -w force1.rest force1.out || exit 1
	./force_test.x < force2.test 
	diff -w force2.rest force2.out || exit 1 
	./force_test.x < force3.test 
	diff -w force3.rest force3.out || exit 1

integration_test: integration_test.x
	./integration_test.x || exit 1 
