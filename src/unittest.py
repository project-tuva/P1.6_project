import unittest

class Test(unittest.TestCase):

    def test_force(self):
        self.assertEqual('foo'.upper(), 'FOO')

    def Integration(self):
        self.assert()
            
    def test_kinetic(self):
        self.assertTrue('FOO'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_input(self):
        s = 'hello world'
        self.assertEqual()
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)

if __name__ == '__main__':
    unittest.main()
