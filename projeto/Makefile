run:
	cc -Wall -c -o ppos_core.o ppos_core.c
	cc -Wall -c -o queue.o queue.c 
	
	cc -Wall -c -o pingpong-contab-prio.o pingpong-contab-prio.c
	cc pingpong-contab-prio.o ppos_core.o queue.o -o pingpong-contab-prio

	cc -Wall -c -o pingpong-maintask.o pingpong-maintask.c
	cc pingpong-maintask.o ppos_core.o queue.o -o pingpong-maintask

debug:
	cc -Wall -DDEBUG -g -c -o ppos_core.o ppos_core.c
	cc -Wall -DDEBUG -g -c -o queue.o queue.c

	cc -Wall -DDEBUG -g -c -o pingpong-contab-prio.o pingpong-contab-prio.c
	cc pingpong-contab-prio.o ppos_core.o queue.o -o pingpong-contab-prio
	
	cc -Wall -DDEBUG -g -c -o pingpong-maintask.o pingpong-maintask.c
	cc pingpong-maintask.o ppos_core.o queue.o -o pingpong-maintask

purge:
	rm -f *.o
	rm -f a.out
	rm -f pingpong-contab
	rm -f pingpong-contab-prio
	rm -f pingpong-maintask