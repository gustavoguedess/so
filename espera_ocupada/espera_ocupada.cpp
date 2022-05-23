#include<stdio.h>
#include <pthread.h>
#include"queue.c"
#include<stdlib.h>
#include<time.h>

#define NUM_THREADS  2

typedef struct elemint_t
{
    struct elemint_t *prev;
    struct elemint_t *next ;
    int id;
}elemint_t; 

elemint_t *fila;
long int turn;
int wants[2] = {0,0};

int retira_primeiro_elemento_da_fila(){
    int id;
    elemint_t* primeiro = fila;
    queue_remove((queue_t **)&fila, (queue_t *) primeiro);
    id = primeiro->id;
    free(primeiro);
    return id;
}

void poe_elemento_no_fim_da_fila(int n){
    elemint_t *novo = (elemint_t*) malloc(sizeof(elemint_t));
    novo->id = n;
    novo->next = NULL;
    novo->prev = NULL;
    queue_append( (queue_t **)&fila, (queue_t*) novo );
}


void init(int tam){
    printf("inicio do init\n");
    srand(time(NULL));
    
    fila = NULL;
    for(int i=0; i<tam; i++){
        int novo = rand()%100;
        poe_elemento_no_fim_da_fila(novo);
    }
    printf("fim do init\n");
}

void print_fila(){
    elemint_t *current = fila;
    do{
        printf("%d ", current->id);
        current = current->next;
    }while(current!=fila);
    printf("\n");
}

void enter_cs(long int id){
    int other = 1-id;
    wants[id] = 1;
    turn = other;
    while((turn==other) && wants[other]){}
}
void leave(long int id){
    wants[id]=0;
}

void *threadBody(void *id){
    int velho, novo;
    while(true){
        enter_cs((long int) id);

        velho = retira_primeiro_elemento_da_fila();
        novo = rand()%100;
        poe_elemento_no_fim_da_fila(novo);
        printf("thread %ld: tira %d,  põe %d,  fila: ", (long int) id, velho, novo);
        print_fila();

        leave((long int) id);
    }
}

int main (int argc, char *argv[])
{
    init(10);
    pthread_t thread [NUM_THREADS] ;
    pthread_attr_t attr ;
    long i, status ;
    
    pthread_attr_init (&attr) ;
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ;
    
    // create threads
    for(i=0; i<NUM_THREADS; i++)
    {
        status = pthread_create (&thread[i], &attr, threadBody, (void *) i) ;
        if (status)
        {
            perror ("pthread_create") ;
            exit (1) ;
        }
    }
    
    // wait all threads to finish   
    for (i=0; i<NUM_THREADS; i++)
    {
        status = pthread_join (thread[i], NULL) ;
        if (status)
        {
            perror ("pthread_join") ;
            exit (1) ;
        }
    }

    pthread_attr_destroy (&attr) ;
    pthread_exit (NULL) ;
}
