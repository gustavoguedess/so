run:
	cc -Wall -c -o queue.o ../filas/queue.c
	cc -Wall -c -o semaforo.o semaforo.cpp
	cc -Wall -c -o produtor_consumidor.o produtor_consumidor.cpp

	cc queue.o semaforo.o -o semaforo
	cc queue.o produtor_consumidor.o -o produtor_consumidor

rm:
clean:
	rm -f queue.o
	rm -f semaforo.o semaforo
	rm -f produtor_consumidor.o produtor_consumidor