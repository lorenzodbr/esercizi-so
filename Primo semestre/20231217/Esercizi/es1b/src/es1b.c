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

//Svolgere lo stesso esercizio utilizzando memoria condivisa e processi, e non più le pthreads come 
//nel primo caso.

//struttura per stabilire i limiti nei quali calcolare la somma
typedef struct {
    int from;
    int to;
} bounds_t;

//struttura per la memoria condivisa
struct shared {
    int partialSums[N];
};

//array di numeri da sommare
int numbers[N] = {0};

int sum(bounds_t);

int main(int argc, char *argv[]){
    int processCount = 0;                           //numero di processi da generare
    int i;                                          //iteratore
    int mainSum = 0, processSum = 0, exitCode;      //somme

    pid_t pid[N];                                   //array di pid per i processi
    bounds_t bounds[N];                             //array di bounds per i processi

    //creazione della memoria condivisa
    int shmId = shmget(IPC_PRIVATE, sizeof(struct shared), IPC_CREAT | S_IRUSR | S_IWUSR);
    
    //controllo della creazione della memoria condivisa
    if (shmId == -1){
        printf("Impossibile creare la shared memory!\n");
        exit(1);
    }

    printf("Inserimento nell'array:\n");
    for(i = 0; i < N; i++){                         //inserimento dei numeri nell'array
        printf("numbers[%d] = ", i);
        scanf("%d", &numbers[i]);
        mainSum += numbers[i];                      //calcolo della somma monoprocesso
    }

    printf("Numero di processi: ");         
    scanf("%d", &processCount);                     //inserimento del numero di processi da generare

    printf("Somma monoprocesso: %d\n", mainSum);
        
    if(processCount > N){                           //se il numero di processi è maggiore del numero di elementi dell'array
        processCount = N;                           //è inutile generare più processi di quanti sono gli elementi dell'array
    }

    //attacco la memoria condivisa al processo
    struct shared *sharedMemoryPointer = shmat(shmId, 0, 0);

    //controllo dell'attacco della memoria condivisa
    if (sharedMemoryPointer == (void *) -1){
        printf("shmat failed");
        exit(1);
    }

    //creazione dei processi
    for(i = 0; i < processCount; i++){
        //calcolo dei bounds
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

        //creo un nuovo processo
        pid[i] = fork();

        //controllo della creazione del processo
        if(pid[i] < 0){
            printf("Errore nella creazione del processo %d\n", i);
            exit(1);
        } else if(pid[i] == 0){     //se sono il processo figlio calcolo, salvo il risultato ed esco
            sharedMemoryPointer->partialSums[i] = sum(bounds[i]);
            exit(0);
        }
    }

    //attendo che tutti i processi finiscano
    for(i = 0; i < processCount; i++){
        waitpid(pid[i], &exitCode, 0);
    }

    //calcolo della somma multiprocesso
    for(i = 0; i < processCount; i++){
        processSum += sharedMemoryPointer->partialSums[i];
    }

    //rimuovo la memoria condivisa
    if (shmdt(sharedMemoryPointer) == -1){
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