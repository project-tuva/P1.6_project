SHELL=/bin/sh

ljmd.so: #force_test.c

# gcc -c force_compute.c -o force_compute.o -fpic #####
# gcc -c utilities.c -o utilities.o -fpic #####
# gcc -c verlet_1.c  -o verlet_1.o -fpic #####
# gcc -c verlet_2.c -o verlet_2.o -fpic #####
# gcc -c input.c  -o input.o -fpic #####
# gcc -c output.c -o output.o -fpic #####

# gcc -shared force_compute.o utilities.o verlet_1.o verlet_2.o input.o output.o -o ljmd.X.so
# OR I CAN CALL THEM FROM DIRECTORY?! NO BECAUSE OF -FPIC FLAG! 

clean:
	rm -f *~ *.so *.o
