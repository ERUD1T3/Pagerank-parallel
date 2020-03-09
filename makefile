pagerank:
	gcc -o ./bin/pg ./c_files/main.c

debug:
	gcc -ggdb -o ./bin/pg_debug ./c_files/main.c

run:
	./bin/pg