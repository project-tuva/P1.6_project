#!/usr/bin/env python

import unittest
from ctypes import *

class mdsys(Structure):
    _fields_ =[("natoms", c_int),("nfi", c_int),("nsteps", c_int),\
               ("dt",c_double),("mass",c_double),("epsilon",c_double),("sigma",c_double),("box",c_double),("rcut",c_double),\
               ("ekin",c_double),("epot",c_double),("temp",c_double),\
               ("rx",POINTER(c_double)),("ry",POINTER(c_double)),("rz",POINTER(c_double)),\
               ("vx",POINTER(c_double)),("vy",POINTER(c_double)),("vz",POINTER(c_double)),\
               ("fx",POINTER(c_double)),("fy",POINTER(c_double)),("fz",POINTER(c_double))]

# I should have input as number of atom for allocation of types
# definition of struct and another for testing
# the interface should be just for heavier functions as force, ververlet.
    

# import shared object
dso = CDLL("./ljmd.so")

print ("Calling DSO")

with open('argon_108.inp', 'r') as input:
    for line in input:
        sys = line.spilit('#')
#    input.readline() 





class Test(unittest.TestCase):

    def test_force(self):
        dso.force()
        self.assertEqual('foo'.upper(), 'FOO')

    def test_Integration(self):
        dso.verlet_1()
        dso.verlet_2()
        self.assert()
            
    def test_kinetic(self):
        dso.ekin()
        self.assertTrue()
        self.assertFalse()

    def test_input(self):
        self.assertEqual()
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)

if __name__ == '__main__':
    unittest.main()
