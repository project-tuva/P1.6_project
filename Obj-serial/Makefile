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
CFLAGS=-Wall -O3 -ffast-math -pg -g -I$(HEADDIR)
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
