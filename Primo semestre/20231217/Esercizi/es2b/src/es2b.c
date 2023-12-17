#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROCESS_COUNT 8

// Svolgere lo stesso esercizio utilizzando memoria condivisa e processi, e non più le pthreads come 
// nel primo caso.

typedef struct {
    int from;
    int to;
} bounds_t;

struct shared {
    int partialFattorials[PROCESS_COUNT];
};

int fatt(bounds_t);
long fattMain(int);

int main(int argc, char *argv[]){
    int n, i;
    long processFatt = 1;
    int exitCode;

    pid_t pid[PROCESS_COUNT];

    int shmId = shmget(IPC_PRIVATE, sizeof(struct shared), IPC_CREAT | S_IRUSR | S_IWUSR);
    
    if (shmId == -1){
        printf("Impossibile creare la shared memory!\n");
        exit(1);
    }

    printf("Numero di cui calcolare il fattoriale: ");
    scanf("%d", &n);

    printf("Fattoriale monothread: %ld\n", fattMain(n));

    bounds_t bounds[PROCESS_COUNT];

    struct shared *shMemPointer = shmat(shmId, 0, 0);

    if (shMemPointer == (void *) -1){
        printf("shmat failed");
        exit(1);
    }

    for(i = 0; i < n && i < PROCESS_COUNT; i++){
        if(i > 0){
            bounds[i].from = bounds[i - 1].to + 1;
        } else {
            bounds[i].from = 1;
        }

        bounds[i].to = bounds[i].from + floor(n / PROCESS_COUNT);

        if(n >= PROCESS_COUNT){
            bounds[i].to -= 1;
        }

        if(i == PROCESS_COUNT - 1){
            bounds[i].to = n;
        }

        pid[i] = fork();

        if(pid[i] < 0){
            printf("Errore nella creazione del processo %d\n", i);
            exit(1);
        } else if(pid[i] == 0){
            shMemPointer->partialFattorials[i] = fatt(bounds[i]);
            exit(0);
        }
    }

    for(i = 0; i < n && i < PROCESS_COUNT; i++){
        waitpid(pid[i], &exitCode, 0);
    }

    for(i = 0; i < n && i < PROCESS_COUNT; i++){
        processFatt *= shMemPointer->partialFattorials[i];
    }

    if (shmdt(shMemPointer) == -1){
        printf("Unlink shared memory fallito!\n");
        exit(1);
    }

    printf("Fattoriale multiprocesso: %ld\n", processFatt);

    return 0;
}

int fatt(bounds_t bounds){

    long fatt = 1;

    for(int i = bounds.from; i <= bounds.to; i++){
        fatt *= i;
    }

    return fatt;
}

long fattMain(int n){
    long fatt = 1;

    for(int i = 1; i <= n; i++){
        fatt *= i;
    }

    return fatt;
}