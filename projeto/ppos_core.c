
#include "queue.h"
#include "ppos_data.h"
#include "ppos.h"
#include<stdlib.h>
#include<stdio.h>

task_t *fila, *current;
task_t dispatcher_task;
int next_id;

#define STACKSIZE 64*1024	/* tamanho de pilha das threads */

void dispatcher ();

/* Inicializa o sistema*/
void ppos_init(){
    setvbuf (stdout, 0, _IONBF, 0);

    
    #ifdef DEBUG
        printf("DEBUG task_init: Iniciando...\n");
    #endif


    fila = NULL;
    current = NULL;
    next_id = 0;

    task_t main;
    task_create(&main, dispatcher, "main");

    task_create(&dispatcher_task, dispatcher, "Dispatcher");

    current = fila;

    #ifdef DEBUG
        printf("DEBUG task_init: Iniciado com sucesso\n");
    #endif
}

/*Cria uma nova tarefa*/
int task_create (task_t *task, void (*start_routine)(void *),  void *arg){
    getcontext (&task->context) ;

    char *stack;
    stack = malloc (STACKSIZE) ;
    if (stack)
    {
        task->context.uc_stack.ss_sp = stack ;
        task->context.uc_stack.ss_size = STACKSIZE ;
        task->context.uc_stack.ss_flags = 0 ;
        task->context.uc_link = 0 ;
    }
    else
    {
        perror ("Erro na criação da pilha: ") ;
        return -1;
    }

    makecontext (&task->context, (void*)(*start_routine), 1, arg);

    task->id = next_id++;
    task->prev = NULL;
    task->next = NULL;
    task->status = 0;
    //task->preemptable = 0;

    queue_append((queue_t **) &fila, (queue_t*) task);

    #ifdef DEBUG
    printf("DEBUG task_create: criou a tarefa %d\n", task->id);
    #endif

    return 0;
}

/* Transfere o processador para outra tarefa */
int task_switch(task_t *task){

    
    #ifdef DEBUG
        printf("DEBUG task_switch: Trocando de contexto da tarefa %d para %d\n", current->id, task->id);
    #endif
    task_t *aux = current;
    current = task;
    swapcontext (&aux->context, &task->context);

    return 0;
}


/* Finaliza a tarefa exit_code*/
void task_exit(int exit_code){
    #ifdef DEBUG
        printf("DEBUG task_exit: Finalizando a tarefa %d\n", current->id);
    #endif

    queue_remove((queue_t**) &fila, (queue_t*) current);
    task_switch(fila);
}

int task_id (){
    if(current==NULL) return -1;
    return current->id;
}

task_t* scheduler(){
    task_t* aux = (&dispatcher_task)->next;
    while(aux->id<2) aux = aux->next;
    return aux;
}

void task_yield(){
    queue_remove((queue_t**)&fila, (queue_t*)current);
    queue_append((queue_t**)&fila, (queue_t*)current);
    task_switch(&dispatcher_task);
}

void dispatcher (){
    
    task_t *next;
    
    while( queue_size((queue_t*)fila)>2 ){
        next = scheduler();
        if(next != NULL){
            task_switch(next);
            switch(next->status){
                case 0: //Pronta
                    break;
                case 1: //Terminada
                    break;
                case 2: //Suspensa
                    break;
            }
        }
        
    }
    #ifdef DEBUG
        printf("DEBUG dispatcher: tchau!\n");
    #endif
    
    
    task_exit(0);
}