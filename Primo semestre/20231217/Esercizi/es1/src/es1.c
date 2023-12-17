#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>

#define N 10

// Scrivere un programma C che calcoli la somma di un array di numeri interi in parallelo utilizzando pthread. Il 
// programma deve prendere in ingresso un array di numeri interi e il numero di thread. Ogni thread deve 
// essere responsabile della somma di una parte dell'array e il thread principale deve attendere che tutti i 
// thread finiscano utilizzando pthread_join prima di stampare il risultato finale.

typedef struct {
    int from;
    int to;
    int result;
} bounds_t;

int numbers[N] = {0};

void *sum(void *);

int main(int argc, char *argv[]){
    int threadCount = 0, i;
    int mainSum = 0, threadSum = 0;
    int *threadResult;
    int exitCode;

    pthread_t tid[N];

    printf("Inserimento nell'array:\n");
    for(i = 0; i < N; i++){
        printf("numbers[%d] = ", i);
        scanf("%d", &numbers[i]);
        mainSum += numbers[i];
    }

    printf("Numero di thread: ");
    scanf("%d", &threadCount);

    printf("Somma monothread: %d\n", mainSum);

    bounds_t bounds[N];
        
    if(threadCount > N){
        threadCount = N;
    }

    for(i = 0; i < threadCount; i++){
        if(i > 0)
            bounds[i].from = bounds[i - 1].to + 1;
        else {
            bounds[i].from = 0;
        }
        
        if(i == threadCount - 1){
            bounds[i].to = N - 1;
        } else if(N - bounds[i].from > threadCount - i){
            bounds[i].to = bounds[i].from + (N - bounds[i].from) / (threadCount - i);
        } else {
            bounds[i].to = bounds[i].from + (N / threadCount) - 1;
        }
        
        exitCode = pthread_create(&tid[i], NULL, sum, &bounds[i]);

        if(exitCode != 0){
            printf("Errore nella creazione del thread %d\n", i);
            exit(1);
        }
    }

    for(i = 0; i < threadCount; i++){
        exitCode = pthread_join(tid[i], (void **) &threadResult);

        if(exitCode != 0){
            printf("Errore nella join del thread %d\n", i);
            exit(1);
        } else {
            threadSum += bounds[i].result;
        }
    }

    printf("Somma multithread: %d\n", threadSum);

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