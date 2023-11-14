#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MIN 2
#define MAX 32
#define PATH "/home/lorenzo/Scrivania/SO/Primo semestre/20231113/Esercizi/es4/bin/es4_a"

// Scrivere:
//   a) un programma che legge da riga di comando due interi n, m. Il programma 
//      stampa a video la stringa: “Il prodotto di n e m e’: x“, dove x e’ il risultato 
//      dell’operazione n*m
//   b) un secondo programma che genera due numeri casuali. Mediante la “system 
//      call” exec, il programma utilizza il programma descritto nel punto a) per 
//      calcolarne il prodotto.

int main(int argc, char * argv[]){
    srand(time(NULL) ^ getpid());
    char n[12]; 
    char m[12];
    sprintf(n, "%d", MIN + rand() % (MAX - MIN));
    sprintf(m, "%d", MIN + rand() & (MAX - MIN));

    execl(PATH, "es4_a", n, m, NULL);
    
    return -1;  //eseguita solo in caso di errore della exec
}
