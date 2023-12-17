#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>

#define THREAD_COUNT 8

// Scrivere un programma C che calcoli il fattoriale di un dato numero in parallelo utilizzando pthread. Il 
// programma deve prendere in input un numero intero e ogni thread deve essere responsabile del calcolo di 
// una parte del fattoriale. Il thread principale deve attendere che tutti i thread finiscano utilizzando 
// pthread_join prima di stampare il risultato finale.

//struttura per stabilire i limiti nei quali calcolare il fattoriale
typedef struct {
    int from;
    int to;
    int result;
} bounds_t;

void *fatt(void *);
long fattMono(int);

int main(int argc, char *argv[]){
    int n;                  //numero di cui calcolare il fattoriale
    int i;                  //iteratore
    long threadFatt = 1;    //fattoriale calcolato dai thread
    int exitCode;           //codice di uscita

    pthread_t tid[THREAD_COUNT];    //array di tid per i thread

    //inserimento del numero di cui calcolare il fattoriale
    printf("Numero di cui calcolare il fattoriale: ");
    scanf("%d", &n);

    //calcolo del fattoriale monoprocesso
    printf("Fattoriale monothread: %ld\n", fattMono(n));

    //creazione dei bounds per i thread
    bounds_t bounds[THREAD_COUNT];

    for(i = 0; i < n && i < THREAD_COUNT; i++){
        //calcolo dei bounds
        if(i > 0){
            bounds[i].from = bounds[i - 1].to + 1;
        } else {
            bounds[i].from = 1;
        }

        bounds[i].to = bounds[i].from + floor(n / THREAD_COUNT);

        if(n >= THREAD_COUNT){
            bounds[i].to -= 1;
        }

        if(i == THREAD_COUNT - 1){
            bounds[i].to = n;
        }

        //creo il thread con la funzione fatt e i bounds
        exitCode = pthread_create(&tid[i], NULL, fatt, &bounds[i]);

        //controllo la creazione del thread
        if(exitCode != 0){
            printf("Errore nella creazione del thread %d\n", i);
            exit(1);
        }
    }

    for(i = 0; i < THREAD_COUNT && i < n; i++){
        //attendo la terminazione del thread
        exitCode = pthread_join(tid[i], NULL);

        if(exitCode != 0){
            printf("Errore nella join del thread %d\n", i);
            exit(1);
        }
    }

    for(i = 0; i < THREAD_COUNT && i < n; i++){
        //calcolo il fattoriale totale
        threadFatt *= bounds[i].result;
    }

    printf("Fattoriale multithread: %ld\n", threadFatt);

    pthread_exit(NULL);
    return 0;
}

//funzione per calcolare il fattoriale con i bounds passati
void *fatt(void *arg){
    bounds_t *bounds = (bounds_t *) arg;

    long fatt = 1;

    for(int i = bounds->from; i <= bounds->to; i++){
        fatt *= i;
    }

    bounds->result = fatt;

    pthread_exit(NULL);
}

//funzione per calcolare il fattoriale tradizionalmente
long fattMono(int n){
    long fatt = 1;

    for(int i = 1; i <= n; i++){
        fatt *= i;
    }

    return fatt;
}