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

typedef struct {
    int from;
    int to;
    int result;
} bounds_t;

void *fatt(void *);
long fattMain(int);

int main(int argc, char *argv[]){
    int n, i;
    long threadFatt = 1;
    int *threadResult;
    int exitCode;

    pthread_t tid[THREAD_COUNT];

    printf("Numero di cui calcolare il fattoriale: ");
    scanf("%d", &n);

    printf("Fattoriale monothread: %ld\n", fattMain(n));

    bounds_t bounds[THREAD_COUNT];

    for(i = 0; i < n && i < THREAD_COUNT; i++){
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

        printf("Thread %d: %d - %d\n", i, bounds[i].from, bounds[i].to);

        exitCode = pthread_create(&tid[i], NULL, fatt, &bounds[i]);

        if(exitCode != 0){
            printf("Errore nella creazione del thread %d\n", i);
            exit(1);
        }
    }

    for(i = 0; i < THREAD_COUNT && i < n; i++){
        exitCode = pthread_join(tid[i], (void **) &threadResult);

        if(exitCode != 0){
            printf("Errore nella join del thread %d\n", i);
            exit(1);
        } else {
            threadFatt *= bounds[i].result;
        }
    }

    printf("Fattoriale multithread: %ld\n", threadFatt);

    pthread_exit(NULL);
    return 0;
}

void *fatt(void *arg){
    bounds_t *bounds = (bounds_t *) arg;

    long fatt = 1;

    for(int i = bounds->from; i <= bounds->to; i++){
        fatt *= i;
    }

    bounds->result = fatt;

    pthread_exit(NULL);
}

long fattMain(int n){
    long fatt = 1;

    for(int i = 1; i <= n; i++){
        fatt *= i;
    }

    return fatt;
}