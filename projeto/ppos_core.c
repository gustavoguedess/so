
#include "queue.h"
#include "ppos_data.h"
#include "ppos.h"
#include<stdlib.h>
#include<stdio.h>

#include <signal.h>
#include <sys/time.h>

task_t *fila, *current_task;
task_t dispatcher_task, main_task;
int next_id;

#define STACKSIZE 64*1024	/* tamanho de pilha das threads */

struct sigaction action ;
struct itimerval timer;

int tick, quantum;
int current_time;
void dispatcher ();

void tick_handler(int signum){
    tick--;
    #ifdef DEBUG
        //printf("INTERROMPI!!! tick: %d  task: %d system: %d\n", tick, current_task->id, current_task->system);
    #endif
    current_task->ticks++;
    current_time++;
    if(current_task->system==1){
        tick = quantum;
    }
    else if(!tick){
        tick = quantum;
        task_yield();
    }
}

/* Inicializa o sistema*/
void ppos_init(){
    setvbuf (stdout, 0, _IONBF, 0);

    
    #ifdef DEBUG
        printf("DEBUG task_init: Iniciando...\n");
    #endif


    fila = NULL;
    current_task = NULL;
    next_id = 0;

    task_create(&main_task, dispatcher, "main");

    task_create(&dispatcher_task, dispatcher, "Dispatcher");
    dispatcher_task.system=1;

    current_task = fila;

    quantum = 20;
    tick = quantum;
    current_time = 0;

    action.sa_handler = tick_handler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;
    if(sigaction (SIGALRM, &action, 0) < 0){
        perror("Erro em sigaction: ");
        exit(1);
    }
    timer.it_value.tv_usec = 1000;
    timer.it_value.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;
    timer.it_interval.tv_sec = 0;

    if(setitimer (ITIMER_REAL, &timer, 0)<0){
        perror("Erro em setitimer: ");
        exit(1);
    }

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
    task->start = systime();
    task->ticks = 0;
    task->activations = 0;
    task->end = 0;

    queue_append((queue_t **) &fila, (queue_t*) task);

    #ifdef DEBUG
        printf("DEBUG task_create: criou a tarefa %d\n", task->id);
    #endif

    return 0;
}

/* Transfere o processador para outra tarefa */
int task_switch(task_t *task){

    
    #ifdef DEBUG
        printf("DEBUG task_switch: Trocando de contexto da tarefa %d para %d\n", current_task->id, task->id);
    #endif
    task_t *aux = current_task;
    current_task = task;
    task->activations++;
    swapcontext (&aux->context, &task->context);

    return 0;
}


/* Finaliza a tarefa exit_code*/
void task_exit(int exit_code){
    #ifdef DEBUG
        printf("DEBUG task_exit: Finalizando a tarefa %d\n", current_task->id);
    #endif
    queue_remove((queue_t**) &fila, (queue_t*) current_task);
    current_task->end = systime();
    printf("Task %d exit: execution time %d ms, processor time %d ms, %d activations\n", current_task->id, current_task->end-current_task->start, current_task->ticks, current_task->activations);
    if(current_task!=&dispatcher_task)
        task_switch(&dispatcher_task);
    
}

int task_id (){
    if(current_task==NULL) return -1;
    return current_task->id;
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
    if(current_task->system==0){
        task_setprio(current_task, current_task->pe);
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
        current_task->pe = prio;
        current_task->pd = prio;
    }
    else{
        task->pe = prio;
        task->pd = prio;
    }

}

int task_getprio (task_t *task){
    if (task==NULL)
        return current_task->pe;
    return task->pe;
}

unsigned int systime () {
    return current_time;    
}
