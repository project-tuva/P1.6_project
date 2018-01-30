import os
import sys
from ctypes import *

dso  = CDLL("./ljmd.X.so")
print ("Calling DSO \n")

nprint = c_int
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
#os.dup2(fd_rpipe, sys.stdin.fileno())

def test_force():
    with open('force1.test', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:
        c_stdin.write(input_file.read())
    dso.set_mdsys(byref(sy),restfile,trajfile,ergfile,line,byref(nprint))
    dso.allocate_mdsys(byref(sy))
    dso.set_ic(byref(sy),restfile)
    dso.force(byref(sy))
    dso.test_output(byref(sy),erg,traj)#should be subsitute with python function
#    assert 
    dso.free_mdsys(byref(sy)) 
        
def test_Integration():        
    dso.set_mdsys(byref(sy),restfile,trajfile,ergfile,line,byref(nprint))
    dso.allocate_mdsys(byref(sy))
    dso.set_ic(byref(sy),restfile)
    dso.verlet_1(byref(sy))
    dso.verlet_2(byref(sy))
    dso.test_output(byref(sy),erg,traj)
#    assert
    dso.free_mdsys(byref(sy))

            
def test_kinetic():
    with open('kinetic1.test', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:
        c_stdin.write(input_file.read())
    dso.set_mdsys(byref(sy),restfile,trajfile,ergfile,line,byref(nprint))
    dso.allocate_mdsys(byref(sy))
    dso.set_ic_f(byref(sy),restfile)
    dso.ekin(byref(sy))
    dso.test_output(byref(sy),erg,traj)
#    assert
    dso.free_mdsys(byref(sy))


def test_input():
#    os.dup2(fd_rpipe, sys.stdin.fileno())
#    with open('input_test.inp', 'r') as input_file, os.fdopen(fd_wpipe, 'w') as c_stdin:
#        c_stdin.write(input_file.read())

    with open('input_test.inp', 'r') as input_file:
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
        mdsys.rcut = float( l.split("#")[0].strip() )
        l = input_file.readline()
        mdsys.box = float( l.split("#")[0].strip() )
        l = input_file.readline()
        restfile = l.split("#")[0].strip()[:200]
        l = input_file.readline()
        trajfile = l.split("#")[0].strip()[:200]
        l = input_file.readline()
        regfile = l.split("#")[0].strip()[:200]
        l = input_file.readline()
        mdsys.nsteps = int( l.split("#")[0].strip() )
        l = input_file.readline()
        mdsys.dt = float( l.split("#")[0].strip() )
        l = input_file.readline()
        sys.nfi = int( l.split("#")[0].strip() )    
#    dso.set_mdsys(byref(sy),restfile,trajfile,ergfile,line,byref(nprint))

    with open('input_test.out', 'w') as output_file:
        output_file.write("%d\t" % (sys.natoms));
        output_file.write("# natoms\n");
        output_file.write("%.3f\t" % (sys.mass));
        output_file.write("# mass in AMU\n");
        output_file.write("%.4f\t" % (sys.epsilon));
        output_file.write("# epsilon in kcal/mol\n");
        output_file.write("%.3f\t" % (sys.sigma));
        output_file.write("# sigma in angstrom\n");
        output_file.write("%.1f\t" % (sys.rcut));
        output_file.write("# rcut in angstrom\n");
        output_file.write("%.4f\t" % (sys.box));
        output_file.write("# box length (in angstrom)\n");
        output_file.write("%s\t" % restfile);
        output_file.write("# restart\n");
        output_file.write("%s\t" % trajfile);
        output_file.write("# trajectory\n");
        output_file.write("%s\t" % ergfile);
        output_file.write("# energies\n");
        output_file.write("%d\t" % (sys.nsteps));
        output_file.write("# nr MD steps\n");
        output_file.write("%.1f\t" % (sys.dt));
        output_file.write("# MD time step (in fs)\n");
        output_file.write("%d\t" % (sys.nfi));
        output_file.write("# output print frequency\n");
  
    assert input_file == output_file
    
