#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 10

// Svolgere lo stesso esercizio utilizzando memoria condivisa e processi, e non più le pthreads come 
// nel primo caso.

typedef struct {
    int from;
    int to;
} bounds_t;

struct shared {
    int partialSums[N];
};

int numbers[N] = {0};

int sum(bounds_t);

int main(int argc, char *argv[]){
    int processCount = 0, i;
    int mainSum = 0, processSum = 0, exitCode;

    pid_t pid[N];

    int shmId = shmget(IPC_PRIVATE, sizeof(struct shared), IPC_CREAT | S_IRUSR | S_IWUSR);
    
    if (shmId == -1){
        printf("Impossibile creare la shared memory!\n");
        exit(1);
    }

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

    struct shared *shMemPointer = shmat(shmId, 0, 0);

    if (shMemPointer == (void *) -1){
        printf("shmat failed");
        exit(1);
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

        pid[i] = fork();

        if(pid[i] < 0){
            printf("Errore nella creazione del processo %d\n", i);
            exit(1);
        } else if(pid[i] == 0){
            shMemPointer->partialSums[i] = sum(bounds[i]);
            exit(0);
        }
    }

    for(i = 0; i < processCount; i++){
        waitpid(pid[i], &exitCode, 0);
    }

    for(i = 0; i < processCount; i++){
        processSum += shMemPointer->partialSums[i];
    }

    if (shmdt(shMemPointer) == -1){
        printf("Unlink shared memory fallito!\n");
        exit(1);
    }

    printf("Somma multiprocesso: %d\n", processSum);

    return 0;
}

int sum(bounds_t bounds){
    int i, sum = 0;

    for(i = bounds.from; i <= bounds.to; i++){
        sum += numbers[i];
    }

    return sum;
}