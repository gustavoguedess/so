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

void enter(){
    int velho, novo;
    while(true){
        velho = retira_primeiro_elemento_da_fila();
        novo = rand()%100;
        poe_elemento_no_fim_da_fila(novo);
        printf("thread %d: tira %d,  põe %d,  fila: ", 0, velho, novo);
        print_fila();
    }
}

int main(){

    init(10);
    
    enter();
    return 0; 
}