#include<stdio.h>
#include"queue.c"
#include<stdlib.h>
#include<time.h>

typedef struct elemint_t
{
    struct elemint_t *prev;
    struct elemint_t *next ;
    int id;
}elemint_t; 

elemint_t *fila;

int retira_primeiro_elemento_da_fila(){
    int id;
    printf("Removendo primeiro da fila\n");
    elemint_t* primeiro = fila;
    queue_remove((queue_t **)fila, (queue_t *) primeiro);
    id = primeiro->id;
    printf("Antes do Free\n");
    free(primeiro);
    printf("Retornando\n");
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

void enter(){
    int velho, novo;
    for(int i=0; i<1; i++){
        velho = retira_primeiro_elemento_da_fila();
        printf("Gerando");
        novo = rand()%100;
        poe_elemento_no_fim_da_fila(novo);
        printf("thread %d: tira %d,  põe %d,  fila: ", 0, velho, novo);
    }
}

int main(){

    init(10);

    enter();
    return 0; 
}