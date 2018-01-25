#Dirs
ROOT=..
EXEDIR=$(ROOT)
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -g -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR)
LDLIBS=-lm

MAIN=$(ROOT)/main

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/ljmd-serial.x
#default:
#	echo $(OBJ)
#	echo $(SRC)


clean:
	rm -f *.mod *.o $(EXEDIR)/ljmd-serial.x $(OBJ)

# linker rule
$(EXEDIR)/ljmd-serial.x: $(OBJ) $(MAIN).o
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@  

$(MAIN).o: $(MAIN).c
	$(CC) -c $< $(CFLAGS) -o $(MAIN).o

# dependencies
#ljmd.o: ljmd.c
