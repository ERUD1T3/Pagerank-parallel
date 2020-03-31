all: pgrk1 pgrk2 pgrk3 pgrkX test

test:
	gcc -ggdb -o ./bin/t ./c_files/test.c

pgrk1:
	gcc -fopenmp -o ./bin/pgrk1 ./c_files/assgn1.c

pgrk2:
	gcc -fopenmp -o ./bin/pgrk2 ./c_files/assgn2.c

pgrk3:
	mpicc -o ./bin/pgrk3 ./c_files/assgn3.c

pgrkX:
	mpicc -o ./bin/pgrkx ./c_files/assgnextra.c

runt: 
	./bin/t 16

run1:
	./bin/pgrk1 16

run2:
	./bin/pgrk2 16

run3:
	mpiexec ./bin/pgrk3 16

runX:
	mpiexec ./bin/pgrkx 16