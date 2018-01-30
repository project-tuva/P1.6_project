#Dirs
ROOT=..
EXEDIR=$(ROOT)
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include
LIBDIR=$(ROOT)/Obj-serial

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc

CFLAGS=-Wall -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
LDLIBS=-lm -pg -no-pie -L$(LIBDIR) -Wl,-rpath,Obj-serial -Wl,-rpath,../Obj-serial

MAIN=$(ROOT)/main

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/ljmd-serial.x

#default:
#	echo $(OBJ)
#	echo $(SRC)

clean:
	rm -f *.mod *.o *~ $(EXEDIR)/ljmd-serial.x $(OBJ) $(EXEDIR)/*.o 

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h 

%.o: %.c
	$(CC) -c -fPIC $< $(CFLAGS) -o $@  

$(LIBDIR)/libserial.so: $(OBJ)
	$(CC) -shared $(OBJ) -o $(LIBDIR)/libserial.so

$(EXEDIR)/ljmd-serial.x: $(MAIN).c $(LIBDIR)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o $(EXEDIR)/ljmd-serial.x $(MAIN).c -lserial

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
# Case 6  (-O3, -ffast-math, avoid expensive math, Newton 3rd law, clang)
#CC=clang
#CFLAGS=-Wall  -std=c99 -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 7 (-O3, doesn't know -ffast-math, avoid expensive math, Newton 3rd law, gcc default)
#CFLAGS=-Wall -O3 -ffast-math -pg -g -fno-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
# Case 8 (-O3, doesn't know -ffast-math, avoid expensive math, Newton 3rd law, gcc default, inline working)
#CFLAGS=-Wall -O3 -ffast-math -pg -g -no-pie -I$(HEADDIR)
#--------------------------------------------------------------------------
