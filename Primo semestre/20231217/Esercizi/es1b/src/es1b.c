#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 10

// Svolgere lo stesso esercizio utilizzando memoria condivisa e processi, e non più le pthreads come 
// nel primo caso.

typedef struct {
    int from;
    int to;
    int result;
} bounds_t;

int numbers[N] = {0};

void *sum(void *);

int main(int argc, char *argv[]){
    int processCount = 0, i;
    int mainSum = 0, processSum = 0;
    int *processResult;
    int exitCode;

    pid_t pid[N];

    printf("Inserimento nell'array:\n");
    for(i = 0; i < N; i++){
        printf("numbers[%d] = ", i);
        scanf("%d", &numbers[i]);
        mainSum += numbers[i];
    }

    printf("Numero di processi: ");
    scanf("%d", &processCount);

    printf("Somma monoprocesso: %d\n", mainSum);

    bounds_t bounds[N];
        
    if(processCount > N){
        processCount = N;
    }

    for(i = 0; i < processCount; i++){
        if(i > 0)
            bounds[i].from = bounds[i - 1].to + 1;
        else {
            bounds[i].from = 0;
        }
        
        if(i == processCount - 1){
            bounds[i].to = N - 1;
        } else if(N - bounds[i].from > processCount - i){
            bounds[i].to = bounds[i].from + (N - bounds[i].from) / (processCount - i);
        } else {
            bounds[i].to = bounds[i].from + (N / processCount) - 1;
        }

        printf("Thread %d: %d - %d\n", i, bounds[i].from, bounds[i].to);
    }

    printf("Somma multiprocesso: %d\n", processSum);

    pthread_exit(NULL);
    return 0;
}