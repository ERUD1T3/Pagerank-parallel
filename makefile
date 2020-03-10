pagerank:
	gcc -o ./bin/pgrk ./c_files/main.c

debug:
	gcc -ggdb -o ./bin/pgrk_debug ./c_files/main.c

run:
	./bin/pgrk