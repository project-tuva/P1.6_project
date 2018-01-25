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

MAIN=$(ROOT)/main.c

OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/ljmd-serial.x

clean:
	rm -f *.mod *.o $(EXEDIR)/ljmd-serial.x $(SRCDIR)/*.o

# linker rule
$(EXEDIR)/ljmd-serial.x: $(OBJ)
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

# compilation pattern rule for objects
%.o: $(HEADDIR)/ljmd.h

%.o: %.c
	$(CC) -c $(CFLAGS) $<

# dependencies
#ljmd.o: ljmd.c
