#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "../filas/queue.c"

#define NUM_PRODUTORES 3
#define NUM_CONSUMIDORES 2

sem_t s_buffer, s_item, s_vaga ;

typedef struct elemint_t
{
    struct elemint_t *prev;
    struct elemint_t *next ;
    int id;
}elemint_t; 

elemint_t *buffer;

int retira_primeiro_elemento_da_fila(){
    int id;
    elemint_t* primeiro = buffer;
    queue_remove((queue_t **)&buffer, (queue_t *) primeiro);
    id = primeiro->id;
    free(primeiro);
    return id;
}

void poe_elemento_no_fim_da_fila(int n){
    elemint_t *novo = (elemint_t*) malloc(sizeof(elemint_t));
    novo->id = n;
    novo->next = NULL;
    novo->prev = NULL;
    queue_append( (queue_t **)&buffer, (queue_t*) novo );
}

void print_fila(){
    elemint_t *current = buffer;
    do{
        printf("%d ", current->id);
        current = current->next;
    }while(current!=buffer);
    printf("\n");
}

void* produtor(void *id){
    int pause, item;
    while(true){
        pause = rand()%3;
        sleep(pause);
    }

    pthread_exit (NULL) ;
}
void* consumidor(void *id){
    printf("\tCONSUMIDOR\n");

    pthread_exit (NULL) ;
}

void create_threads(pthread_t pthread[], int n,
    void *(*f)(void*), pthread_attr_t *attr){
    long i, status;
    // create threads
    for(i=0; i<n; i++)
    {
        status = pthread_create (&pthread[i], attr, f, (void *) i) ;
        if (status)
        {
            perror ("pthread_create") ;
            exit (1) ;
        }
    }
}
void join_threads(pthread_t pthread[], int n){  
    long i, status; 
    // wait all threads to finish   
    for (i=0; i<n; i++)
    {
        status = pthread_join (pthread[i], NULL) ;
        if (status)
        {
            perror ("pthread_join") ;
            exit (1) ;
        }
    }
}
int main (int argc, char *argv[])
{
    srand (time(NULL));
    pthread_t thread_produtores [NUM_PRODUTORES] ;
    pthread_t thread_consumidores [NUM_CONSUMIDORES] ;
    
    sem_init (&s_buffer, 0, 1) ;
    sem_init (&s_item, 0, 1) ;
    sem_init (&s_vaga, 0, 1) ;


    pthread_attr_t attr ;
    pthread_attr_init (&attr) ;
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ;
    
    create_threads(thread_produtores, NUM_PRODUTORES, produtor, &attr);
    create_threads(thread_consumidores, NUM_CONSUMIDORES, consumidor, &attr);
    
    join_threads(thread_consumidores, NUM_CONSUMIDORES);
    join_threads(thread_produtores, NUM_PRODUTORES);
    
    pthread_attr_destroy (&attr) ;
    pthread_exit (NULL) ;
}