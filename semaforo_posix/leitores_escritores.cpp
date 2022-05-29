#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_LEITORES 3
#define NUM_ESCRITORES 2
#define TIME_RAND 3

int num_leitores=0;
pthread_mutex_t marea, mcont;

void* leitor(void* id){
    printf("*LEITOR*\n");

    int pause;
    while(true){
        pthread_mutex_lock(&mcont);
        num_leitores++;
        if(num_leitores==1) 
            pthread_mutex_lock(&marea);
        pthread_mutex_unlock(&mcont);

        pause = rand()%TIME_RAND;
        printf("LEITOR %ld:\tnum_leitores %d\n", (long int)id, num_leitores);
        sleep(pause);

        pthread_mutex_lock(&mcont);
        num_leitores--;
        if(num_leitores==0)
            pthread_mutex_unlock(&marea);
        pthread_mutex_unlock(&mcont);

        pause = rand()%TIME_RAND*3;
        sleep(pause);
    }

    pthread_exit(NULL);
}

void* escritor(void* id){
    printf("*ESCRITOR*\n");

    int pause;
    while(true){
        pthread_mutex_lock(&marea);
        pause = rand()%TIME_RAND;
        printf("ESCRITOR %ld\n", (long int)id);
        sleep(pause);
        pthread_mutex_unlock(&marea);
        pause = rand()%TIME_RAND;
        sleep(pause);
    }

    pthread_exit(NULL);
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

int main(int argc, char *argv[]){
    srand(time(NULL));
    
    pthread_mutex_init(&marea, NULL);
    pthread_mutex_init(&mcont, NULL);

    pthread_t thread_leitores[NUM_LEITORES];
    pthread_t thread_escritores[NUM_ESCRITORES];

    pthread_attr_t attr;
    pthread_attr_init (&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    create_threads(thread_leitores, NUM_LEITORES, leitor, &attr);
    create_threads(thread_escritores, NUM_ESCRITORES, escritor, &attr);

    join_threads(thread_leitores, NUM_LEITORES);
    join_threads(thread_escritores, NUM_ESCRITORES);

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}