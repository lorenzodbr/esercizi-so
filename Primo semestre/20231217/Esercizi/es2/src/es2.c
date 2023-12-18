#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>

#define THREAD_COUNT 3

// Scrivere un programma C che calcoli il fattoriale di un dato numero in parallelo utilizzando pthread. Il 
// programma deve prendere in input un numero intero e ogni thread deve essere responsabile del calcolo di 
// una parte del fattoriale. Il thread principale deve attendere che tutti i thread finiscano utilizzando 
// pthread_join prima di stampare il risultato finale.

//struttura per stabilire i limiti nei quali calcolare il fattoriale
typedef struct {
    int from;
    int to;
    long result;
} bounds_t;

void *fatt(void *);
long fattMono(int);
bounds_t *getBounds(int);
void startClock();
void endClock();

clock_t begin = 0, end = 0; //variabili per il calcolo del tempo
int clocksSpent;            //tempo impiegato per i calcoli

int main(int argc, char *argv[]){
    int n;                  //numero di cui calcolare il fattoriale
    int i;                  //iteratore
    long threadFatt = 1;    //fattoriale calcolato dai thread
    long mainFatt;          //fattoriale calcolato dal main
    int exitCode;           //codice di uscita

    pthread_t tid[THREAD_COUNT];    //array di tid per i thread

    //inserimento del numero di cui calcolare il fattoriale
    printf("Numero di cui calcolare il fattoriale: ");
    scanf("%d", &n);

    startClock();
    mainFatt = fattMono(n);     //calcolo del fattoriale monoprocesso
    endClock();

    //calcolo del fattoriale monoprocesso
    printf("Fattoriale monothread: %ld in %d clock\n", mainFatt, clocksSpent);

    //----- THREADS -----

    //creazione dei bounds per i thread
    bounds_t *bounds = getBounds(n);

    if(!bounds){
        printf("Errore nell'allocazione della memoria!\n");
        exit(1);
    }

    startClock();

    for(i = 0; i < n && i < THREAD_COUNT; i++){
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

    endClock();

    printf("Fattoriale multithread: %ld in %d clock\n", threadFatt, clocksSpent);

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

bounds_t * getBounds(int howMany){
    bounds_t *bounds = (bounds_t *) malloc(howMany * sizeof(bounds_t));

    if(!bounds) return NULL;

    int i;
    for(i = 0; i < howMany; i++){
        //calcolo dei bounds
        if(i > 0){
            bounds[i].from = bounds[i - 1].to + 1;
        } else {
            bounds[i].from = 1;
        }

        bounds[i].to = bounds[i].from + floor(howMany / THREAD_COUNT);

        if(howMany >= THREAD_COUNT){
            bounds[i].to -= 1;
        }

        if(i == THREAD_COUNT - 1){
            bounds[i].to = howMany;
        }
    }

    return bounds;
}

void startClock(){
    begin = clock();
}

void endClock(){
    end = clock();
    clocksSpent = end - begin;
}