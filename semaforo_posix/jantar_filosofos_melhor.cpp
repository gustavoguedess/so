#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define N NUM_FILOSOFOS
#define ESQUERDA (i+N-1)%N 
#define DIREITA (i+1)%N
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2

sem_t filosofos[NUM_FILOSOFOS];
pthread_mutex_t mut, saleiro;
int num_refeicoes;
int estado[N];

void meditar(){
    int pause = rand()%2;
    sleep(pause);
}
void comer(long id){
    pthread_mutex_lock(&mut);
    num_refeicoes++;
    pthread_mutex_unlock(&mut);
    int pause = rand()%2;
    sleep(pause);
}
void teste(long i){
    if(estado[i]==FAMINTO && estado[ESQUERDA]!=COMENDO && estado[DIREITA]!=COMENDO){
        estado[i] = COMENDO;
        sem_post(&filosofos[i]);
    }
}
void pegar_garfos(long i){
    pthread_mutex_lock(&saleiro);
    estado[i] = FAMINTO;
    teste(i);
    pthread_mutex_unlock(&saleiro);
    sem_wait(&filosofos[i]);
}
void soltar_garfos(long i){
    pthread_mutex_lock(&saleiro);
    estado[i] = PENSANDO;
    teste(ESQUERDA);
    teste(DIREITA);
    pthread_mutex_unlock(&saleiro);
}
void* filosofo(void* id){
    printf("FILOSOFO\n");

    while(true){
        meditar();
        pegar_garfos((long)id);
        comer((long) id);
        soltar_garfos((long)id);        
    }
    pthread_exit(NULL);
}

void create_threads(pthread_t pthread[], int n, void *(*f)(void*), pthread_attr_t *attr){
    long i, status;
    for(i=0; i<n; i++){
        status = pthread_create(&pthread[i], attr, f, (void*) i);
        if(status){
            perror("pthread_create");
            exit(1);
        }
    }
}

void join_threads(pthread_t pthread[], int n){
    long i, status;
    for(i=0; i<n; i++){
        status = pthread_join(pthread[i], NULL);
        if(status){
            perror("pthread_join");
            exit(1);
        }
    }
}
int main(){
    srand(time(NULL));
    for(int i=0; i<NUM_FILOSOFOS; i++)
        sem_init(&filosofos[i], 0, 1);
    pthread_mutex_init(&mut, NULL);
    pthread_mutex_init(&saleiro, NULL);
    num_refeicoes = 0;

    pthread_t thread_filosofos[NUM_FILOSOFOS];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    create_threads(thread_filosofos, NUM_FILOSOFOS, filosofo, &attr);

    //join_threads(thread_filosofos, NUM_FILOSOFOS);

    while(true){
        sleep(1);
        pthread_mutex_lock(&mut);
        printf("Refeições por segundo: %d\n",num_refeicoes);
        num_refeicoes=0;
        pthread_mutex_unlock(&mut);
    }

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}