#include <stdio.h>
#include <stdlib.h>

#define N 1
#define M 2

// Scrivere:
//   a) un programma che legge da riga di comando due interi n, m. Il programma 
//      stampa a video la stringa: “Il prodotto di n e m e’: x“, dove x e’ il risultato 
//      dell’operazione n*m
//   b) un secondo programma che genera due numeri casuali. Mediante la “system 
//      call” exec, il programma utilizza il programma descritto nel punto a) per 
//      calcolarne il prodotto.

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("usage: es4_a <n> <m>\n");
        return -1;
    }

    int n = atoi(argv[N]);
    int m = atoi(argv[M]);

    printf("Il prodotto di %d e %d è %d\n", n, m, n*m);

    return 0;
}
