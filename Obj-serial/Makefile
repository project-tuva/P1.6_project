#Dirs
ROOT=..
EXEDIR=$(ROOT)
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -O3 -ffast-math -pg -g -fno-pie -I$(HEADDIR)

LDLIBS=-lm -pg -fno-pie

MAIN=$(ROOT)/main

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/ljmd-serial.x
#default:
#	echo $(OBJ)
#	echo $(SRC)

clean:
	rm -f *.mod *.o $(EXEDIR)/ljmd-serial.x $(OBJ) $(EXEDIR)/*.o 

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
#--------------------------------------------------------------------------
# Original version
#CFLAGS=-Wall -g -std=c99 -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR)
#LDLIBS=-lm
#--------------------------------------------------------------------------
# Case 1 (No optimization)
#CFLAGS=-Wall -std=c99 -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 2 (-O3)
#CFLAGS=-Wall -std=c99 -O3 -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 3 (-O3 and -ffast-math)
#CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 4 (-O3, -ffast-math and avoid expensive math)
#CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 5 (-O3, -ffast-math, avoid expensive math and Newton 3rd law)
#CFLAGS=-Wall  -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 6 (-O3, -ffast-math, avoid expensive math, Newton 3rd law, inline not working) --> WRONG
#CFLAGS=-Wall  -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 7  (-O3, -ffast-math, avoid expensive math, Newton 3rd law, clang)
#CC=clang
#CFLAGS=-Wall  -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 8 (-O3, -ffast-math, avoid expensive math, Newton 3rd law, g++) --> WRONG
#CC=g++
#CFLAGS=-Wall  -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 9 (-O3, doesn't know -ffast-math, avoid expensive math, Newton 3rd law) --> RUN ON LOGIN NODE
#CC=icc (I've to beware of it because it has "unsafe math" option by default)
#CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 10 (-O3, doesn't know -ffast-math, avoid expensive math, Newton 3rd law, gcc default)
#CFLAGS=-Wall -O3 -ffast-math -pg -g -fno-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 11 (-O3, doesn't know -ffast-math, avoid expensive math, Newton 3rd law, gcc default, inline working) --> BEST
#CFLAGS=-Wall -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
