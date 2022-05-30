#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5

sem_t hashi[NUM_FILOSOFOS];
pthread_mutex_t mut;
int num_refeicoes;

void meditar(){
    int pause = rand()%2;
    sleep(pause);
}
void comer(long id){
    int pause = rand()%2;
    sleep(pause);
}
void* filosofo(void* id){
    printf("FILOSOFO\n");

    int dir = (long)id;
    int esq = ((long)id+1)%NUM_FILOSOFOS;

    while(true){
        meditar();
        sem_wait(&hashi[esq]);
        sem_wait(&hashi[dir]);
        comer((long)id);
        pthread_mutex_lock(&mut);
        num_refeicoes++;
        pthread_mutex_unlock(&mut);
        sem_post(&hashi[esq]);
        sem_post(&hashi[dir]);
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
        sem_init(&hashi[i], 0, 1);
    pthread_mutex_init(&mut, NULL);
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