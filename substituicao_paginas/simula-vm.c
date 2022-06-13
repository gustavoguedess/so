#include<stdio.h>
#include <stdlib.h>
#define MAX 1100100

int tam_quadros;
int referencias[MAX];
int tam_referencias;
int faltas_fifo, faltas_lru, faltas_opt;

int simulador_fifo(){
    int quadros[tam_quadros];
    int q=0;
    int achou;
    int fp=0;

    for(int i=0; i<tam_quadros; i++) quadros[i]=-1;
    for(int i=0; i<tam_referencias; i++){
        achou=0;       
        for(int j=0; j<tam_quadros; j++)
            if(referencias[i]==quadros[j]) achou=1;
        if(!achou){
            quadros[q]=referencias[i];
            fp++;
            q = (q+1)%tam_quadros;
        }
    }
    return fp;
}
int simulador_lru(){
    int quadros[tam_quadros];
    int acesso[tam_quadros];
    int achou, pos;
    int fp=0;

    for(int i=0; i<tam_quadros; i++){
        quadros[i] = -1;
        acesso[i] = -1;
    }

    for(int i=0; i<tam_referencias; i++){
        pos = 0;
        achou=0;
        for(int j=0; j<tam_quadros; j++){
            if(acesso[j]<acesso[pos])
                pos = j;
            if(quadros[j]==referencias[i]){
                achou=1;
                pos=j;
                break;
            }
        }
        if(!achou) fp++;
        quadros[pos] = referencias[i];
        acesso[pos] = i;
    }
    return fp;
}
int simulador_opt(){
    int quadros[tam_quadros];
    int pos;
    int distancia;
    int achou;
    int fp=0;

    for(int i=0; i<tam_quadros; i++){
        quadros[i]=-1;
    }
    for(int i=0; i<tam_referencias; i++){
        achou=0;
        distancia=0;
        for(int j=0; j<tam_quadros; j++){
            if(referencias[i]==quadros[j]){
                achou=1;
                pos=j;
                break;
            }
            int k;
            for(k=i; k<tam_referencias && referencias[k]!=quadros[j]; k++);
            if(k>distancia){
                distancia=k;
                pos=j;
            }
        }
        if(!achou) fp++;
        quadros[pos]=referencias[i];
    }
    return fp;
}

int main(int argc, char *argv[]){

    tam_quadros = atoi(argv[1]);

    int i=0;
    while(scanf("%d", &referencias[i])!=EOF)
        i++;
    tam_referencias = i;

    faltas_fifo = simulador_fifo();
    faltas_lru = simulador_lru();
    faltas_opt = simulador_opt();

    printf ("%5d quadros, %7d refs: FIFO: %5d PFs, LRU: %5d PFs, OPT: %5d PFs\n", tam_quadros, tam_referencias, faltas_fifo, faltas_lru, faltas_opt) ;
}