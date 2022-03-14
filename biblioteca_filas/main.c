#include<stdio.h>
#include"queue.h"

int main(){
  queue_t *fila;
  queue_t *elem;
  fila = NULL;
  elem->next=NULL;
  elem->prev=NULL;
  queue_append(&fila, elem);
  printf("%d\n", queue_size(fila));
  queue_remove(&fila, elem);
}
