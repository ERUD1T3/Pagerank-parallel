pagerank:
	gcc -o ./bin/pgrk ./c_files/assgn1.c

assgn2:
	gcc -o ./bin/pgrk2 ./c_files/assgn2.c

assgn3:
	gcc -o ./bin/pgrk3 ./c_files/assgn3.c

assgnextra:
	gcc -o ./bin/pgrkx ./c_files/assgnextra.c

debug:
	gcc -ggdb -o ./bin/pgrk_debug ./c_files/main.c

run:
	./bin/pgrk