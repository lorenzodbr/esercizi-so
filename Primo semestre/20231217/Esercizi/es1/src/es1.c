#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>

#define N 10

// Scrivere un programma C che calcoli la somma di un array di numeri interi in parallelo utilizzando pthread. Il 
// programma deve prendere in ingresso un array di numeri interi e il numero di thread. Ogni thread deve 
// essere responsabile della somma di una parte dell'array e il thread principale deve attendere che tutti i 
// thread finiscano utilizzando pthread_join prima di stampare il risultato finale.

//struttura per stabilire i limiti nei quali calcolare la somma
typedef struct {
    int from;
    int to;
    int result;
} bounds_t;

//array di numeri da sommare
int numbers[N] = {0};

void *sum(void *);
bounds_t *getBounds(int, int);
void startClock();
void endClock();

clock_t begin = 0, end = 0;             //variabili per il calcolo del tempo
int clocksSpent;                        //tempo impiegato per i calcoli

struct timeval t1, t2;                  //variabili per il calcolo del tempo
int timeSpent;                       //tempo impiegato per i calcoli

int main(int argc, char *argv[]){
    int threadCount = 0;                //numero di thread da generare
    int i;                              //iteratore
    long mainSum = 0, threadSum = 0;    //somme (monoprocesso e multithread)
    int exitCode;                       //codice di uscita

    pthread_t tid[N];                   //array di tid per i thread
    bounds_t *bounds;                   //array di bounds per i thread

    printf("Inserimento nell'array:\n");
    for(i = 0; i < N; i++){             //inserimento dei numeri nell'array
        printf("numbers[%d] = ", i);
        scanf("%d", &numbers[i]);
    }

    printf("Numero di thread: ");
    scanf("%d", &threadCount);          //inserimento del numero di thread da generare

    startClock();

    for(i = 0; i < N; i++){
        mainSum += numbers[i];
    }

    endClock();

    //calcolo della somma monoprocesso
    printf("Somma monothread: %ld in %d clock e %d usec\n", mainSum, clocksSpent, timeSpent);
    
    //se il numero di thread è maggiore del numero di elementi dell'array
    //è inutile generare più thread del numero di elementi dell'array
    if(threadCount > N){
        printf("Numero di thread maggiore del numero di elementi dell'array! Imposto al valore di default %d\n", N);
        threadCount = N;
    }

    if(threadCount <= 0){
        printf("Numero di thread non valido! Imposto al valore di default 1\n");
        threadCount = 1;
    }

    //creazione dei bounds per i thread
    bounds = getBounds(0, threadCount);

    //controllo della creazione dei bounds
    if(!bounds){
        printf("Errore nell'allocazione della memoria!\n");
        exit(1);
    }

    startClock();

    for(i = 0; i < threadCount; i++){
        //creo il thread con la funzione sum e i bounds
        exitCode = pthread_create(&tid[i], NULL, sum, &bounds[i]);

        //controllo la creazione del thread
        if(exitCode != 0){
            printf("Errore nella creazione del thread %d\n", i);
            exit(1);
        }
    }

    for(i = 0; i < threadCount; i++){
        //attendo la terminazione del thread
        exitCode = pthread_join(tid[i], NULL);

        //controllo la terminazione del thread
        if(exitCode != 0){
            printf("Errore nella join del thread %d\n", i);
            exit(1);
        }
    }

    //calcolo della somma multithread
    for(i = 0; i < threadCount; i++){
        threadSum += bounds[i].result;
    }

    endClock();

    printf("Somma multithread: %ld in %d clock e %d usec\n", threadSum, clocksSpent, timeSpent);

    free(bounds);
    pthread_exit(NULL);
    return 0;
}

void *sum(void *arg){
    bounds_t *bounds = (bounds_t *) arg;

    int sum = 0;

    for(int i = bounds->from; i <= bounds->to; i++){
        sum += numbers[i];
    }

    bounds->result = sum;

    pthread_exit(NULL);
}

bounds_t * getBounds(int start, int howMany){
    bounds_t *bounds = (bounds_t *) malloc(howMany * sizeof(bounds_t));

    if(bounds){
        int i;
        int elementsPerThread = N / howMany;
        int remainder = N % howMany;

        for(i = 0; i < howMany; i++){
            bounds[i].from = i == 0 ? start : bounds[i - 1].to + 1;
            bounds[i].to = bounds[i].from - 1 + elementsPerThread + (remainder-- > 0 ? 1 : 0);
        }
    }

    return bounds;
}

void startClock(){
    begin = clock();
    gettimeofday(&t1, NULL);
}

void endClock(){
    end = clock();
    gettimeofday(&t2, NULL);
    clocksSpent = end - begin;
    timeSpent = (t2.tv_usec - t1.tv_usec);
}