
#include "queue.h"
#include "ppos_data.h"
#include<stdlib.h>
#include<stdio.h>

task_t *fila, *current;

#define STACKSIZE 64*1024	/* tamanho de pilha das threads */

/* Inicializa o sistema*/
void ppos_init(){
    setvbuf (stdout, 0, _IONBF, 0);

    task_t item;
    item.id = 0;
    item.prev = NULL;
    item.next = NULL;
    //item.status = 0;
    //item.preemptable = 0;

    queue_append((queue_t **) &fila, (queue_t*) &item);
    current = fila;
    #ifdef DEBUG
    printf("DEBUG task_init: Iniciou\n");
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

    task->id = fila->prev->id+1;
    task->prev = NULL;
    task->next = NULL;
    //task->status = 0;
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

    queue_remove((queue_t**) fila, (queue_t*) current);
    task_switch(fila);
}

int task_id (){
    if(current==NULL) return -1;
    return current->id;
}