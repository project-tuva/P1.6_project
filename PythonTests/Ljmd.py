#!/usr/bin/env python

import unittest
from ctypes import *

dso  = CDLL("./ljmd.so")
print ("Calling DSO \n")

nprint = c_int()
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

dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,byref(nprint))

dso.allocate_mdsys(byref(sys))

#dso.set_ic(byref(sys),restfile)

#sys.nfi = c_int(0)
#dso.force(byref(sys))
#dso.ekin(byref(sys))

#open(ergfile,'w') as erg
#open(trajfile, 'w') as traj

#dso.output(byref(sys),erg,traj)

################################

#if ((sys.nfi % nprint) == 0):
#    dso.output(&sys, erg, traj)

#dso.velverlet_1(byref(sys))
#dso.force(byref(sys))
#dso.velverlet_2(byref(sys))
#dso.ekin(byref(sys))

################################

#print("Simulation Done.\n")

#close(erg)
#close(traj)

#dso.free_mdsys(byref(sys))
