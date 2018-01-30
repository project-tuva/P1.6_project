#Dirs
ROOT=..
EXEDIR=$(ROOT)/
SRCDIR=$(ROOT)/src/
HEADDIR=$(ROOT)/include/
LIBDIR=$(ROOT)/Obj-serial/
OBJDIR=../Obj-serial/

# -*- Makefile -*-
SHELL=/bin/sh
CC=gcc
CFLAGS=-Wall -std=c99 -O3 -ffast-math -pg -g -I$(HEADDIR)
LDLIBS=-lm -pg -g -L$(LIBDIR) -Wl,-rpath,Obj-serial -Wl,-rpath,../Obj-serial

MAIN=$(ROOT)/main

SRC=$(wildcard ../src/*.c)
OBJ=$(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)

TARGET=$(EXEDIR)ljmd-serial.x

.PHONY: clean

default: $(TARGET)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) -c -fPIC $(CFLAGS) $<

clean:
	rm -f *.mod *.o $(EXEDIR)ljmd-serial.x $(EXEDIR)*.o $(LIBDIR)libserial.so $(SRCDIR)*.o

$(LIBDIR)libserial.so: $(OBJ)
	$(CC) -shared $(OBJ) -o $(LIBDIR)libserial.so

$(TARGET): $(MAIN).c $(LIBDIR)libserial.so
	$(CC) $(LDLIBS) $(CFLAGS) -o $(EXEDIR)ljmd-serial.x $(MAIN).c -lserial

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
