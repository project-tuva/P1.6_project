In This folder you can find Python interface for Lennard Jones Molecular dynamics code, in this code ctype is used as a c code wrapper in python:

- `Ljmd.py` is an python interface for serial code by using c input function. to run the code `python Ljmd.py` would be enough.
- `Ljmd_v1.py` is an an python interface for serial code as well but handling all the input function in python and parsing the input file by python.
- `Ljmd_mpi.py` is an python interface for MPI version of code, I tried but still there is some problems
- `test_units_pytest.py` file is included four different tests for Force , Integration , Kinetic functions and input data as we did in unit testing in c. all the tests passed. to see the result of test you need to run `python3 -m pytest`. in this test I used pytest. another module for unit testing in python is `unittest`, `tes_units_unittest.py` is written regard to this module, but I didn't have enough time to complete it.