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

jantar_filosofos: queue
	cc -Wall -c -o jantar_filosofos_trivial.o jantar_filosofos_trivial.cpp
	cc queue.o jantar_filosofos_trivial.o -o jantar_filosofos_trivial

	cc -Wall -c -o jantar_filosofos_saleiro.o jantar_filosofos_saleiro.cpp
	cc queue.o jantar_filosofos_saleiro.o -o jantar_filosofos_saleiro

	cc -Wall -c -o jantar_filosofos_melhor.o jantar_filosofos_melhor.cpp
	cc queue.o jantar_filosofos_melhor.o -o jantar_filosofos_melhor

rm:
clean:
	rm -f queue.o
	rm -f semaforo.o semaforo
	rm -f produtor_consumidor.o produtor_consumidor
	rm -f leitores_escritores.o leitores_escritores
	rm -f jantar_filosofos_trivial.o jantar_filosofos_trivial
	rm -f jantar_filosofos_saleiro.o jantar_filosofos_saleiro
	rm -f jantar_filosofos_melhor.o jantar_filosofos_melhor