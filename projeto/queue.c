#include<stdio.h>

typedef struct queue_t
{
   struct queue_t *prev ;  // aponta para o elemento anterior na fila
   struct queue_t *next ;  // aponta para o elemento seguinte na fila
} queue_t ;

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (queue_t *queue) {
  if (queue==NULL)
    return 0;
  queue_t *aux = queue;
  int size = 1;

  while(aux->next!=queue){
    aux = aux->next;
    size+=1;
  }
  return size;
}

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, queue_t *queue, void print_elem (void*) ){
    queue_t *aux = queue;

    printf("[");
    while(aux){
      print_elem(aux);
      aux = aux->next;
      if(aux==queue) aux = NULL;
      else printf(" ");
    }
    printf("]\n");
}

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
// Retorno: 0 se sucesso, <0 se ocorreu algum erro

int queue_append (queue_t **queue, queue_t *elem){
    if (queue == NULL){
      printf("Fila não existe\n");
        return -1;
    }
    if(elem==NULL){
      printf("Elemento não existe\n");
      return -1;
    }
    if(elem->next!=NULL || elem->prev!=NULL){
      printf("Elemento pertence a outra fila\n");
      return -1;
    }
    if ((*queue) == NULL){
      (*queue) = elem;
      (*queue)->next = (*queue);
      (*queue)->prev = (*queue);
    }
    else{
      elem->next = (*queue);
      elem->prev = (*queue)->prev;
      (*queue)->prev->next = elem;
      (*queue)->prev = elem;
    }
    return 0;
}

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: 0 se sucesso, <0 se ocorreu algum erro

int queue_remove (queue_t **queue, queue_t *elem){
  if(queue==NULL){
    printf("Fila não existe\n");
    return -1;
  }
  if((*queue)==NULL){
    printf("Fila vazia\n");
    return -1;
  }
  if(elem==NULL){
    printf("Elemento não existe\n");
    return -1;
  }
  queue_t *aux = (*queue);
  while(aux!=elem && aux->next!=(*queue)){
    aux = aux->next;
  }
  if(aux!=elem){
    printf("Elemento não existe nesta lista\n");
    return -1;
  }

  if(aux==aux->next){
    (*queue) = NULL;
  }
  else{
    aux->prev->next = aux->next;
    aux->next->prev = aux->prev;

    if(aux==(*queue)){
      (*queue)=aux->next;
    }
  }

  aux->next = NULL;
  aux->prev = NULL;

  return 0;
}
