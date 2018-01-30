import os
import sys
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

sy = mdsys()

restfile = create_string_buffer(BLEN)
trajfile = create_string_buffer(BLEN)
ergfile = create_string_buffer(BLEN)
line = create_string_buffer(BLEN)

fd_rpipe, fd_wpipe = os.pipe()
os.dup2(fd_rpipe, sys.stdin.fileno())

def test_force():
    with open('force1.test.inp', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:
        c_stdin.write(input_file.read())
    dso.set_mdsys(byref(sy),restfile,trajfile,ergfile,line,byref(nprint))
    dso.allocate_mdsys(byref(sy))
    dso.set_ic(byref(sy),restfile)
    dso.force(byref(sy))
    dso.test_output(byref(sy),erg,traj)
    #        self.assertEqual('foo'.upper(), 'FOO')
    dso.free_mdsys(byref(sy))
        
def test_Integration():        
    dso.set_mdsys(byref(sy),restfile,trajfile,ergfile,line,byref(nprint))
    dso.allocate_mdsys(byref(sy))
    dso.set_ic(byref(sy),restfile)
    dso.verlet_1(byref(sy))
    dso.verlet_2(byref(sy))
    dso.test_output(byref(sy),erg,traj)
    #        self.assertEqual('foo'.upper(), 'FOO')
    dso.free_mdsys(byref(sy))
    #        self.assert()

            
def test_kinetic():
    with open('kinetic1.test.inp', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:            c_stdin.write(input_file.read())
    dso.set_mdsys(byref(sy),restfile,trajfile,ergfile,line,byref(nprint))
    dso.allocate_mdsys(byref(sy))
    dso.set_ic_f(byref(sy),restfile)
    dso.ekin(byref(sy))
    dso.test_output(byref(sy),erg,traj)
    #        self.assertEqual('input', 'output')
    dso.free_mdsys(byref(sy))
        
#        self.assertTrue()
#        self.assertFalse()

def test_input():
#    os.dup2(fd_rpipe, sys.stdin.fileno())
    with open('input_test.inp', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:
        c_stdin.write(input_file.read())
    dso.set_mdsys(byref(sy),restfile,trajfile,ergfile,line,byref(nprint))
    #        self.assertEqual('argon_108.inp', 'sys') # should be modified!
    #        with self.assertRaises(TypeError):
    #            s.split(2)

if __name__ == '__main__':
    unittest.main()
    
