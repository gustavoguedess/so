run:
	cc -Wall -c -o queue.o queue.c
	cc -Wall -c -o espera_ocupada.o espera_ocupada.cpp

	cc queue.o espera_ocupada.o -o espera_ocupada

rm:
	rm -f queue.o
	rm -f espera_ocupada.o
	rm -f espera_ocupada