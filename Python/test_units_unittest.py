import os
import sys
import unittest
from ctypes import *

dso  = CDLL("./ljmd.X.so")
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

fd_rpipe, fd_wpipe = os.pipe()

class Test(unittest.TestCase):

    def test_force(self,sys):
        os.dup2(fd_rpipe, sys.stdin.fileno())
        with open('force1.test.inp', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:
            c_stdin.write(input_file.read())
        self.dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,byref(nprint))
        self.dso.allocate_mdsys(byref(sys))
        self.dso.set_ic(byref(sys),restfile)
        self.dso.force(byref(sys))
        dso.test_output(byref(sys),erg,traj)
#        self.assertEqual('foo'.upper(), 'FOO')
        self.dso.free_mdsys(byref(sys))

    def test_Integration(self,sys):
        
        self.dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,byref(nprint))
        self.dso.allocate_mdsys(byref(sys))
        self.dso.set_ic(byref(sys),restfile)
        self.dso.verlet_1(byref(sys))
        self.dso.verlet_2(byref(sys))
        dso.test_output(byref(sys),erg,traj)
#        self.assertEqual('foo'.upper(), 'FOO')
        self.dso.free_mdsys(byref(sys))
#        self.assert()

            
    def test_kinetic(self,sys):
        os.dup2(fd_rpipe, sys.stdin.fileno())
        with open('kinetic1.test.inp', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:
            c_stdin.write(input_file.read())
        self.dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,byref(nprint))
        self.dso.allocate_mdsys(byref(sys))
        self.dso.set_ic_f(byref(sys),restfile)
        self.dso.ekin(byref(sys))
        dso.test_output(byref(sys),erg,traj)
#        self.assertEqual('input', 'output')
        self.dso.free_mdsys(byref(sys))
        
#        self.assertTrue()
#        self.assertFalse()

    def test_input(self,sys):
        os.dup2(fd_rpipe, sys.stdin.fileno())
        with open('input_test.inp', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:
            c_stdin.write(input_file.read())
        self.dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,byref(nprint))
#        self.assertEqual('argon_108.inp', 'sys') # should be modified!
#        with self.assertRaises(TypeError):
#            s.split(2)

if __name__ == '__main__':
    unittest.main()
    
