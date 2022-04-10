
#include "queue.c"
#include "ppos_data.h"
#include<stdlib.h>

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

    makecontext (&task->context, (void*)(*start_routine), arg);
}

/* Transfere o processador para outra tarefa */
int task_switch(task_t *task){
    return -1;
}

/* Finaliza a tarefa exit_code*/
void task_exit(int exit_code){

}

int task_id (){
    return -1;
}