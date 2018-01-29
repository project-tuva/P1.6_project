#Dirs
ROOT=..
EXEDIR=$(ROOT)
SRCDIR=$(ROOT)/src
HEADDIR=$(ROOT)/include
LIBDIR=$(ROOT)/Obj-serial

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -g -std=c99 -O3 -ffast-math -fomit-frame-pointer -I$(HEADDIR)
LDLIBS=-lm -L$(LIBDIR) -Wl,-rpath,Obj-serial -Wl,-rpath,../Obj-serial

MAIN=$(ROOT)/main

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:%.c=%.o)

default: $(EXEDIR)/ljmd-serial.x

clean:
	rm -f *.mod *.o $(EXEDIR)/ljmd-serial.x $(OBJ) $(EXEDIR)/*.o 

# compilation pattern rule for objects
$(OBJ): $(HEADDIR)/ljmd.h 

%.o: %.c
	$(CC) -c -fPIC $< $(CFLAGS) -o $@  

$(LIBDIR)/libserial.so: $(OBJ)
	$(CC) -shared $(OBJ) -o $(LIBDIR)/libserial.so

$(EXEDIR)/ljmd-serial.x: $(MAIN).c $(LIBDIR)/libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o $(EXEDIR)/ljmd-serial.x $(MAIN).c -lserial
