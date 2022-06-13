run:
	cc -Wall -c -o simula-vm.o simula-vm.c

	cc simula-vm.o -o simula-vm
debug:
	cc -Wall -DDEBUG -g -c -o simula-vm.o simula-vm.c

purge:
	rm -f *.o
	rm -f a.out
	rm simula-vm