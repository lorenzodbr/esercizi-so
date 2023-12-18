#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

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
bounds_t *getBounds(int, int);
void startClock();
void endClock();

clock_t begin = 0, end = 0;             //variabili per il calcolo del tempo
int clocksSpent;                        //tempo impiegato per i calcoli

struct timeval t1, t2;                  //variabili per il calcolo del tempo
int timeSpent;                       //tempo impiegato per i calcoli

int main(int argc, char *argv[]){
    int processCount = 0;                           //numero di processi da generare
    int i;                                          //iteratore
    long mainSum = 0, processSum = 0;               //somme
    int exitCode;                                   //codice di uscita

    pid_t pid[N];                                   //array di pid per i processi
    bounds_t *bounds;                               //array di bounds per i processi

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
    }

    printf("Numero di processi: ");         
    scanf("%d", &processCount);                     //inserimento del numero di processi da generare

    if(processCount <= 0){
        printf("Numero di processi non valido! Imposto al valore di default 1\n");
        processCount = 1;
    }

    if(processCount > N){
        printf("Numero di processi maggiore del numero di elementi dell'array! Imposto al valore di default %d\n", N);
        processCount = N;
    }

    startClock();

    for(i = 0; i < N; i++){
        mainSum += numbers[i];
    }

    endClock();

    printf("Somma monoprocesso: %ld in %d clock e %d usec\n", mainSum, clocksSpent, timeSpent);
        
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

    bounds = getBounds(0, processCount);

    startClock();

    //creazione dei processi
    for(i = 0; i < processCount; i++){
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

    endClock();

    //rimuovo la memoria condivisa
    if (shmdt(sharedMemoryPointer) == -1){
        printf("Unlink shared memory fallito!\n");
        exit(1);
    }

    printf("Somma multiprocesso: %ld in %d clock e %d usec\n", processSum, clocksSpent, timeSpent);

    return 0;
}

int sum(bounds_t bounds){
    int i, sum = 0;

    for(i = bounds.from; i <= bounds.to; i++){
        sum += numbers[i];
    }

    return sum;
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