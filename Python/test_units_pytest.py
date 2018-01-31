<<<<<<< HEAD
import os
import sys
=======

>>>>>>> python
from ctypes import *

#dso  = CDLL("./ljmd.X.so")
dso  = CDLL("./libserial.so")
print ("Calling DSO \n")

<<<<<<< HEAD
nprint = c_int()
=======
nprint = c_int
>>>>>>> python
BLEN = 200

class mdsys(Structure):
    _fields_ =[("natoms", c_int),("nfi", c_int),("nsteps", c_int),\
               ("dt",c_double),("mass",c_double),("epsilon",c_double),("sigma",c_double),("box",c_double),("rcut",c_double),\
               ("ekin",c_double),("epot",c_double),("temp",c_double),\
               ("rx",POINTER(c_double)),("ry",POINTER(c_double)),("rz",POINTER(c_double)),\
               ("vx",POINTER(c_double)),("vy",POINTER(c_double)),("vz",POINTER(c_double)),\
               ("fx",POINTER(c_double)),("fy",POINTER(c_double)),("fz",POINTER(c_double))]

<<<<<<< HEAD
sy = mdsys()

=======
>>>>>>> python
restfile = create_string_buffer(BLEN)
trajfile = create_string_buffer(BLEN)
ergfile = create_string_buffer(BLEN)
line = create_string_buffer(BLEN)

<<<<<<< HEAD
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
    
=======
def input_func(ifile):
    with open(ifile, 'r') as input_file:
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
        restfile = bytes(l.split("#")[0].strip()[:200], 'utf8')
        l = input_file.readline()
        trajfile = bytes(l.split("#")[0].strip()[:200], 'utf8')
        l = input_file.readline()
        ergfile = bytes(l.split("#")[0].strip()[:200], 'utf8')
        l = input_file.readline()
        sys.nsteps = int( l.split("#")[0].strip() )
        l = input_file.readline()
        sys.dt = float( l.split("#")[0].strip() )
        l = input_file.readline()
        sys.nfi = int( l.split("#")[0].strip() ) 
    return (sys,restfile,trajfile,ergfile)


def test_force():
    
    sys1,restfile1,trajfile1,ergfile1 = input_func('force1.test')
#    sys2,restfile2,trajfile2,ergfile2 = input_func('../tests/force2.test')
#    sys3,restfile3,trajfile3,ergfile3 = input_func('../tests/force3.test')
    dso.allocate_mdsys(byref(sys1))
    dso.set_ic_f(byref(sys1),restfile1)
    dso.force(byref(sys1))

    with open(trajfile1, 'w') as traj:  
        print( "Starting force test with %d atoms for %d steps.\n" % (sys1.natoms, sys1.nsteps))
        for i in range(sys1.natoms):
            print("%.5f %.5f %.5f\n" % (sys1.rx[i], sys1.ry[i], sys1.rz[i]))
            traj.write("%.5f %.5f %.5f\n" % (sys1.rx[i], sys1.ry[i], sys1.rz[i]))
        for i in range(sys1.natoms):
            traj.write("%.5f %.5f %.5f\n" % (sys1.vx[i], sys1.vy[i], sys1.vz[i]))
            print("%.5f %.5f %.5f\n" % (sys1.rx[i], sys1.ry[i], sys1.rz[i]))
        for i in range(sys1.natoms):
            traj.write("%.5f %.5f %.5f\n" % (sys1.fx[i], sys1.fy[i], sys1.fz[i]))
            print("%.5f %.5f %.5f\n" % (sys1.rx[i], sys1.ry[i], sys1.rz[i]))

    with open('force1.out') as f1:
        forc1 = f1.read()
    with open('force1.rest') as fr1:
        forcr1 = fr1.read()

    assert forc1 == forcr1
    dso.free_mdsys(byref(sys1))
'''   
    with open('force2.out') as f2:
        forc2 = f2.read()
    with open('force2.rest') as fr2:
        forcr2 = fr1.read()
    with open('force3.out') as f3:
        forc3 = f3.read()
    with open('force3.rest') as fr3:
        forcr3 = fr3.read()
'''

        
def test_Integration():
    
    sys,restfile,trajfile,ergfile = input_func('Integration1.test')
#    sys2,restfile2,trajfile2,ergfile2 = input_func('../tests/Integration2.test')
#    sys3,restfile3,trajfile3,ergfile3 = input_func('../tests/Integration3.test')
    dso.allocate_mdsys(byref(sys))
    dso.set_ic_f(byref(sys),restfile)
    dso.velverlet_1(byref(sys))
    dso.velverlet_2(byref(sys))

    with open(trajfile, 'w') as traj:  
        print( "Starting force test with %d atoms for %d steps.\n" % (sys.natoms, sys.nsteps))
        for i in range(sys.natoms):
            traj.write("%.5f %.5f %.5f\n" % (sys.rx[i], sys.ry[i], sys.rz[i]))
        for i in range(sys.natoms):
            traj.write("%.5f %.5f %.5f\n" % (sys.vx[i], sys.vy[i], sys.vz[i]))
        for i in range(sys.natoms):
            traj.write("%.5f %.5f %.5f\n" % (sys.fx[i], sys.fy[i], sys.fz[i]))

    with open('Integration1.out') as I1:
        integ1 = I1.read()
    with open('Integration1.rest') as Ir1:
        integr1 = Ir1.read()

    assert integ1 == integr1
    dso.free_mdsys(byref(sys))
'''    
    with open('Integration2.out') as I2:
        integ2 = I2.read()
    with open('Integration2.rest') as Ir2:
        ّintegr2 = Ir2.read()
    with open('Integration3.out') as I3:
        integ3 = I3.read()
    with open('Integration3.rest') as Ir3:
        integr3 = Ir3.read()

    assert integ2 == integr2
    assert integ3 == integr3
'''
              
def test_kinetic():

    sys1,restfile1,trajfile1,ergfile1 = input_func('kinetic1.test')
#    sys2,restfile2,trajfile2,ergfile2 = input_func('../tests/kinetic2.test')
#    sys3,restfile3,trajfile3,ergfile3 = input_func('../tests/kinetic3.test')
    dso.allocate_mdsys(byref(sys1))
    dso.set_ic_f(byref(sys1),restfile1)
    dso.ekin(byref(sys1))
    with open(trajfile1, 'w') as traj:  
        print( "Starting force test with %d atoms for %d steps.\n" % (sys1.natoms, sys1.nsteps))
        for i in range(sys1.natoms):
            traj.write("%.5f %.5f %.5f\n" % (sys1.rx[i], sys1.ry[i], sys1.rz[i]))
        for i in range(sys1.natoms):
            traj.write("%.5f %.5f %.5f\n" % (sys1.vx[i], sys1.vy[i], sys1.vz[i]))
        for i in range(sys1.natoms):
            traj.write("%.5f %.5f %.5f\n" % (sys1.fx[i], sys1.fy[i], sys1.fz[i]))

    with open('kinetic1.out') as k1:
        kin1 = k1.read()
    with open('kinetic.rest') as k:
        kinr = k.read()

    assert kin1 == kinr 
    dso.free_mdsys(byref(sys1))
    
'''    with open('kinetic2.out') as k2:
        kin2 = k2.read()
    with open('kinetic3.out') as k3:
        kin3 = k3.read()

'''
    
def test_input():
    sys,restfile,trajfile,ergfile = input_func('input_test.inp')
    sys2,restfile2,trajfile2,ergfile2 = input_func('argon_108.inp')
    assert sys2.natoms == sys.natoms
    assert sys2.mass == sys.mass
    assert sys2.epsilon == sys.epsilon
    assert sys2.sigma == sys.sigma
    assert sys2.rcut == sys.rcut
    assert sys2.box == sys.box
    assert sys2.nsteps == sys.nsteps
    assert sys2.dt == sys.dt
>>>>>>> python
