run:
	cc -Wall -c -o ppos_core.o ppos_core.c
	cc -Wall -c -o queue.o queue.c 
	
	cc -Wall -c -o pingpong-tasks1.o pingpong-tasks1.c
	cc -Wall -c -o pingpong-tasks2.o pingpong-tasks2.c
	cc -Wall -c -o pingpong-tasks3.o pingpong-tasks3.c
	cc -Wall -c -o pingpong-dispatcher.o pingpong-dispatcher.c
	cc pingpong-tasks1.o ppos_core.o queue.o -o pingpong-tasks1
	cc pingpong-tasks2.o ppos_core.o queue.o -o pingpong-tasks2
	cc pingpong-tasks3.o ppos_core.o queue.o -o pingpong-tasks3
	cc pingpong-dispatcher.o ppos_core.o queue.o -o pingpong-dispatcher

debug:
	cc -Wall -DDEBUG -g -c -o ppos_core.o ppos_core.c
	cc -Wall -DDEBUG -g -c -o queue.o queue.c
	
	cc -Wall -DDEBUG -g -c -o pingpong-tasks1.o pingpong-tasks1.c
	cc pingpong-tasks1.o ppos_core.o queue.o -o pingpong-tasks1

	cc -Wall -DDEBUG -g -c -o pingpong-tasks2.o pingpong-tasks2.c
	cc pingpong-tasks2.o ppos_core.o queue.o -o pingpong-tasks2

	cc -Wall -DDEBUG -g -c -o pingpong-tasks3.o pingpong-tasks3.c
	cc pingpong-tasks3.o ppos_core.o queue.o -o pingpong-tasks3

	cc -Wall -DDEBUG -g -c -o pingpong-dispatcher.o pingpong-dispatcher.c
	cc pingpong-dispatcher.o ppos_core.o queue.o -o pingpong-dispatcher
	
purge:
	rm -f *.o
	rm -f a.out
	rm -f pingpong-tasks1
	rm -f pingpong-tasks2
	rm -f pingpong-tasks3
	rm -f pingpong-dispatcher
