all: queue semaforo produtor_consumidor leitores_escritores

queue:
	cc -Wall -c -o queue.o ../filas/queue.c

semaforo:
	cc -Wall -c -o semaforo.o semaforo.cpp
	cc queue.o semaforo.o -o semaforo
	
produtor_consumidor: queue
	cc -Wall -c -o produtor_consumidor.o produtor_consumidor.cpp
	cc queue.o produtor_consumidor.o -o produtor_consumidor

leitores_escritores: queue
	cc -Wall -c -o leitores_escritores.o leitores_escritores.cpp
	cc queue.o leitores_escritores.o -o leitores_escritores

rm:
clean:
	rm -f queue.o
	rm -f semaforo.o semaforo
	rm -f produtor_consumidor.o produtor_consumidor
	rm -f leitores_escritores.o leitores_escritores