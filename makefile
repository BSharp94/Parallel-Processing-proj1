all: run_single run_parallel

run_single: proj1_single.o
	gcc -o run_single proj1_single.o

proj1_single.o: proj1_single.c
	gcc -c proj1_single.c

run_parallel: proj1_parallel.o 
	mpicc -o parallel_exec proj1_parallel.o
	
proj1_parallel.o: proj1_parallel.c
	mpicc -c proj1_parallel.c

