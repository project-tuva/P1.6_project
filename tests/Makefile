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

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

#default: $(EXEDIR)/force_test.x
default: $(EXEDIR)/input_test.x 

clean:
	rm -f *.mod *.o $(EXEDIR)/force_test.x $(EXEDIR)/input_test.x

# linker rule
$(EXEDIR)/force_test.x: $(OBJ) force_test.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

$(EXEDIR)/input_test.x: $(OBJ) input_test.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@  

force_test.o: force_test.c
	$(CC) -c $< $(CFLAGS) -o force_test.o

input_test.o: input_test.c
	$(CC) -c $< $(CFLAGS) -o input_test.o

# dependencies
#ljmd.o: ljmd.c
