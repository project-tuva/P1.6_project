#!/usr/bin/env python

import os
import sys
import unittest
#import numpy as np
from ctypes import *

dso  = CDLL("./ljmd.X.so")
print ("Calling DSO \n")
#sys.argv[1]
fd_rpipe, fd_wpipe = os.pipe()
os.dup2(fd_rpipe, sys.stdin.fileno())
with open('argon_108.inp', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:
    c_stdin.write(input_file.read())

#int(nprint)
nprint = c_int()
#nprint = 0
BLEN = 200

class mdsys(Structure):
    _fields_ =[("natoms", c_int),("nfi", c_int),("nsteps", c_int),\
               ("dt",c_double),("mass",c_double),("epsilon",c_double),("sigma",c_double),("box",c_double),("rcut",c_double),\
               ("ekin",c_double),("epot",c_double),("temp",c_double),\
               ("rx",POINTER(c_double)),("ry",POINTER(c_double)),("rz",POINTER(c_double)),\
               ("vx",POINTER(c_double)),("vy",POINTER(c_double)),("vz",POINTER(c_double)),\
               ("fx",POINTER(c_double)),("fy",POINTER(c_double)),("fz",POINTER(c_double))]

sys = mdsys( )

restfile = create_string_buffer(BLEN)
trajfile = create_string_buffer(BLEN)
ergfile = create_string_buffer(BLEN)
line = create_string_buffer(BLEN)

#dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,c_int(nprint))
#dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,byref(nprint))

#def input(sys,input_file):
with open('argon_108.inp', 'r') as input_file:
    sys = mdsys()
    l = input_file.readline()
    sys.natoms = int( l.split("#")[0].strip() )
    l = input_file.readline()
    sys.mass = float( l.split("#")[0].strip() )
    l = input_file.readline()
    sys.epsilon = float( l.split("#")[0].strip() )
    l = input_file.readline()
    sys.sigma = float( l.split("#")[0].strip() )
    l = input_file.readline()
    sys.rcut = float( l.split("#")[0].strip() )
    l = input_file.readline()
    sys.box = float( l.split("#")[0].strip() )
    l = input_file.readline()
    restfile = l.split("#")[0].strip()[:200]
    l = input_file.readline()
    trajfile = l.split("#")[0].strip()[:200]
    l = input_file.readline()
    ergfile = l.split("#")[0].strip()[:200]
    l = input_file.readline()
    sys.nsteps = int( l.split("#")[0].strip() )
    l = input_file.readline()
    sys.dt = float( l.split("#")[0].strip() )
    l = input_file.readline()
    sys.nfi = int( l.split("#")[0].strip() )

print(sys.natoms)

dso.allocate_mdsys(byref(sys))

dso.set_ic(byref(sys),restfile)

sys.nfi = c_int(0)
dso.force(byref(sys))
dso.ekin(byref(sys))
#I need to open pointer FILE!
i = c_int(0)

print("Starting simulation with %d atoms for %d steps.\n" % (sys.natoms, sys.nsteps))
print("     NFI            TEMP            EKIN                 EPOT              ETOT\n")
#def output(sys,erg,traj)
with open('ergfile', 'w') as erg, open('trajfile', 'w') as traj:
    print(sys.nfi, sys.temp, sys.ekin, sys.epot, sys.ekin+sys.epot, "\n")
    erg.write("% 8d % 20.8f % 20.8f % 20.8f % 20.8f\n" % (sys.nfi, sys.temp, sys.ekin, sys.epot, sys.ekin+sys.epot))
    traj.write("% d\n nfi=%d etot=%20.8f\n" % (sys.natoms, sys.nfi, sys.ekin+sys.epot))
    for i in range(sys.natoms):
        traj.write( "Ar  %20.8f %20.8f %20.8f\n" % (sys.rx[i], sys.ry[i], sys.rz[i]))
        
################################
# main simulation
sys.nfi = c_int(1)
for sys.nfi in range(sys.nsteps):
    if sys.nfi % nprint.value == 0:
        with open('ergfile', 'w') as erg, open('trajfile', 'w') as traj:
            print(sys.nfi, sys.temp, sys.ekin, sys.epot, sys.ekin+sys.epot, "\n")
            erg.write("% 8d % 20.8f % 20.8f % 20.8f % 20.8f\n" % (sys.nfi, sys.temp, sys.ekin, sys.epot, sys.ekin+sys.epot))
            traj.write("% d\n nfi=%d etot=%20.8f\n" % (sys.natoms, sys.nfi, sys.ekin+sys.epot))
            for i in range(sys.natoms):
                traj.write( "Ar  %20.8f %20.8f %20.8f\n" % (sys.rx[i], sys.ry[i], sys.rz[i]))
        
    dso.velverlet_1(byref(sys))
    dso.force(byref(sys))
    dso.velverlet_2(byref(sys))
    dso.ekin(byref(sys))

################################

print("Simulation Done.\n")

dso.free_mdsys(byref(sys))
