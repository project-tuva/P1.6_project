#Dirs
ROOT=..
EXEDIR=.
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -g -std=c99 -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR)
LDLIBS=-lm

MAIN=unit_tests.c

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/unit_tests.x

clean:
	rm -f *.mod *.o $(EXEDIR)/unit_tests.x $(OBJ)

# linker rule
$(EXEDIR)/unit_tests.x: $(OBJ) $(MAIN).o
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@  

$(MAIN).o: $(MAIN).c
	$(CC) -c $< $(CFLAGS) -o $(MAIN).o

# dependencies
#ljmd.o: ljmd.c
