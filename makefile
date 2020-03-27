all: assgn1 assgn2 assgn3 assgnextra test

test:
	gcc -ggdb -o ./bin/t ./c_files/test.c

assgn1:
	gcc -fopenmp -o ./bin/pgrk1 ./c_files/assgn1.c

assgn2:
	gcc -fopenmp -o ./bin/pgrk2 ./c_files/assgn2.c

assgn3:
	mpicc -o ./bin/pgrk3 ./c_files/assgn3.c

assgnextra:
	mpicc -o ./bin/pgrkx ./c_files/assgnextra.c

# debug:
# 	gcc -ggdb -o ./bin/pgrk_debug ./c_files/main.c

# run:
# 	./bin/pgrk

