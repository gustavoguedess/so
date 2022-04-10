run:

	cc -Wall -c -o ppos_core.o ppos_core.c
	cc -Wall -c -o queue.o queue.c
	
	cc -Wall -c -o pingpong-tasks1.o pingpong-tasks1.c
	cc -Wall -c -o pingpong-tasks2.o pingpong-tasks2.c
	cc -Wall -c -o pingpong-tasks3.o pingpong-tasks3.c
	cc pingpong-tasks1.o ppos_core.o queue.o -o pingpong-tasks1
	cc pingpong-tasks2.o ppos_core.o queue.o -o pingpong-tasks2
	cc pingpong-tasks3.o ppos_core.o queue.o -o pingpong-tasks3

debug:
	cc -Wall -DDEBUG -g -c -o ppos_core.o ppos_core.c
	cc -Wall -DDEBUG -g -c -o queue.o queue.c
	
	cc -Wall -DDEBUG -g -c -o pingpong-tasks1.o pingpong-tasks1.c
	cc pingpong-tasks1.o ppos_core.o queue.o -o pingpong-tasks1

	cc -Wall -DDEBUG -g -c -o pingpong-tasks2.o pingpong-tasks2.c
	cc pingpong-tasks2.o ppos_core.o queue.o -o pingpong-tasks2

	cc -Wall -DDEBUG -g -c -o pingpong-tasks3.o pingpong-tasks3.c
	cc pingpong-tasks3.o ppos_core.o queue.o -o pingpong-tasks3
	
purge:
	rm -f *.o
	rm -f a.out
	rm -f pingpong-tasks{1,2,3}
