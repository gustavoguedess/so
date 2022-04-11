
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
    main.system = 1;

    task_create(&dispatcher_task, dispatcher, "Dispatcher");
    dispatcher_task.system=1;

    current = fila;

    #ifdef DEBUG
        printf("DEBUG task_init: Iniciado com sucesso\n");
        printf("DEBUG main id: %d system task: %d\n", fila->id, fila->system);
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
    task->system = 0;
    task->pe = 0;
    task->pd = 0;
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
    if(current==&dispatcher_task)
        task_switch(fila);
    else{
        task_switch(&dispatcher_task);
    }
}

int task_id (){
    if(current==NULL) return -1;
    return current->id;
}

void print_elem(task_t *task){
    printf("%d:%d/%d", task->id, task->pe, task->pd);
}

task_t* scheduler(){
    task_t* next_task = NULL;
    task_t* aux = fila;
    int alpha = -1;

    do{
        if(aux->system==0) {
            aux->pd+=alpha;
            if(next_task==NULL) next_task = aux;
            if(aux->pd < next_task->pd ) next_task = aux;
        }
        aux = aux->next;
    }while(aux!=fila);

    #ifdef DEBUG
        queue_print("", (queue_t*)fila, (void *)print_elem);
    #endif

    return next_task;
}

void task_yield(){
    if(current->system==0){
        task_setprio(current, current->pe);
    }
    task_switch(&dispatcher_task);
}

short has_task(){
    task_t* aux = fila;
    short has_task=0;
    do{
        if (aux->system==0) has_task=1;
        aux = aux->next;
    }while(aux!=fila);

    return has_task;
}
void dispatcher (){
    
    task_t *next;
    
    while( has_task() ){
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

void task_setprio (task_t *task, int prio) {
    if(task==NULL){
        current->pe = prio;
        current->pd = prio;
    }
    else{
        task->pe = prio;
        task->pd = prio;
    }

}

int task_getprio (task_t *task){
    if (task==NULL)
        return current->pe;
    return task->pe;
}