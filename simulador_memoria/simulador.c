#include "../filas/queue.c"
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <windows.h>

#define STACKSIZE 20

int memoria[STACKSIZE];
int mem_livre;
int frag_menor, frag_maior, frag_medio;
int mem_id;

void calc_mem(){
    frag_menor = STACKSIZE;
    frag_maior = 0;

    int frag_qt_tot=0;
    int frag_tam_tot=0;
    int frag_id_atual=-1;
    int frag_tam_atual=-1;

    mem_livre=0;
    for(int i=0; i<STACKSIZE; i++){
        if(memoria[i]==-1)
            mem_livre+=1;
        if(memoria[i]!=frag_id_atual){
            if(frag_id_atual!=-1){
                frag_tam_tot+=frag_tam_atual;
                frag_qt_tot+=1;
                if(frag_tam_atual<frag_menor)
                    frag_menor = frag_tam_atual;
                if(frag_tam_atual>frag_maior)
                    frag_maior = frag_tam_atual;
            }
            frag_id_atual = memoria[i];
            frag_tam_atual=0;
        }
        frag_tam_atual+=1;
    }
    if(frag_qt_tot==0) {
        frag_qt_tot=1;
        frag_menor=0;
    }
    frag_medio = frag_tam_tot/frag_qt_tot;
}

void init(){
    srand(time(NULL));
    mem_id = 1;
    mem_livre = STACKSIZE;
    for (int i=0; i<STACKSIZE; i++)
        memoria[i]=-1;
    calc_mem();
}

void aloca_firstfit(int tam){
    int comeco=0;

    int qt=0;
    for(int i=0; i<STACKSIZE; i++){
        if(memoria[i]==-1){
            qt++;
        }
        else{
            qt=0;
            comeco=i+1;
        }
        if(qt==tam){
            break;
        }
    }
    if(comeco==STACKSIZE){
        return;
    }
    int id_frag = mem_id++;
    for(int i=comeco; i<comeco+tam; i++)
        memoria[i]=id_frag;
    printf("O Fragmento %d foi adicionado na posição %d com tamanho %d\n", id_frag, comeco, tam);
    calc_mem();
}
void desaloca(int id_frag){
    for(int i=0; i<STACKSIZE; i++)
        if(memoria[i]==id_frag) 
            memoria[i]=-1;
    calc_mem();
}

void print_mem(){
    for(int i=0; i<STACKSIZE; i++)
        printf("%3.d", memoria[i]);
}
void memoria_info(){
    printf("Memória Total: %d\n", STACKSIZE);
    printf("Memória Livre: %d\n", mem_livre);
    printf("Fragmento menor: %d\n", frag_menor);
    printf("Fragmento maior: %d\n", frag_maior);
    printf("Fragmento medio: %d\n", frag_medio);
    printf("Memória: ");
    print_mem();
    printf("\n\n");
}

int main(){
    int tam;
    init();
    memoria_info();

    while(true){
        tam = rand()%5;
        aloca_firstfit(tam);
        memoria_info();
        Sleep(2000);
    }
}