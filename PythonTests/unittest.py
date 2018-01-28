import unittest

class Test(unittest.TestCase):

    def test_force(self,sys):
        dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,byref(nprint))
        dso.allocate_mdsys(byref(sys))
        dso.set_ic(byref(sys),restfile)
        dso.force(byref(sys))
        dso.test_output(byref(sys),erg,traj)
        self.assertEqual('foo'.upper(), 'FOO')
        dso.free_mdsys(byref(sys))

    def test_Integration(self,sys):
        dso.verlet_1(byref(sys))
        dso.verlet_2(byref(sys))
        self.assert()
            
    def test_kinetic(self,sys):
        dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,byref(nprint))
        dso.allocate_mdsys(byref(sys))
        dso.set_ic_f(byref(sys),restfile)
        dso.ekin(byref(sys))
        dso.test_output(byref(sys),erg,traj)
        self.assertEqual('input', 'output')
        dso.free_mdsys(byref(sys))
        
        self.assertTrue()
        self.assertFalse()

    def test_input(self,sys):
        dso.set_mdsys(byref(sys),restfile,trajfile,ergfile,line,byref(nprint))
        open
        self.assertEqual('argon_108.inp', 'sys') # should be modified!
        with self.assertRaises(TypeError):
            s.split(2)

if __name__ == '__main__':
    unittest.main()
    
