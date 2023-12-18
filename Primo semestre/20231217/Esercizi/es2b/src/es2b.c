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

#define PROCESS_COUNT 4

//Svolgere lo stesso esercizio utilizzando memoria condivisa e processi, e non più le pthreads come 
//nel primo caso.

//Struttura per stabilire i limiti nei quali calcolare il fattoriale
typedef struct {
    int from;
    int to;
} bounds_t;

//Struttura per la memoria condivisa
typedef struct {
    int partialFattorials[PROCESS_COUNT];
} shared;

int fatt(bounds_t);
long fattMono(int);
bounds_t *getBounds(int, int);
void startClock();
void endClock();

clock_t begin = 0, end = 0;         //variabili per il calcolo del tempo
int clocksSpent;                    //tempo impiegato per i calcoli

struct timeval t1, t2;              //variabili per il calcolo del tempo
int timeSpent;                      //tempo impiegato per i calcoli

int main(int argc, char *argv[]){
    int n;                          //numero di cui calcolare il fattoriale
    int i;                          //iteratore
    long long processFatt = 1;      //fattoriale calcolato dai processi
    long long mainFatt;             //fattoriale calcolato dal main
    int exitCode;                   //codice di uscita

    pid_t pid[PROCESS_COUNT];       //array di pid per i processi
    bounds_t *bounds;               //array di bounds per i processi

    //creazione della memoria condivisa
    int shmId = shmget(IPC_PRIVATE, sizeof(shared), IPC_CREAT | S_IRUSR | S_IWUSR);
    
    //controllo della creazione della memoria condivisa
    if (shmId == -1){
        printf("Impossibile creare la shared memory!\n");
        exit(1);
    }

    //inserimento del numero di cui calcolare il fattoriale
    printf("Numero di cui calcolare il fattoriale: ");
    scanf("%d", &n);

    startClock();
    mainFatt = fattMono(n);
    endClock();

    //calcolo del fattoriale monoprocesso
    printf("Fattoriale monoprocesso: %lld in %d clock e %d usec\n", mainFatt, clocksSpent, timeSpent);

    //attacco la memoria condivisa al processo
    shared *sharedMemoryPointer = shmat(shmId, 0, 0);

    //controllo dell'attacco della memoria condivisa
    if (sharedMemoryPointer == (void *) -1){
        printf("shmat failed");
        exit(1);
    }

    bounds = getBounds(1, n);

    if(!bounds){
        printf("Errore nell'allocazione della memoria!\n");
        exit(1);
    }

    startClock();

    //creazione dei processi
    for(i = 0; i < n && i < PROCESS_COUNT; i++){
        //creo un nuovo processo
        pid[i] = fork();

        //controllo della creazione del processo
        if(pid[i] < 0){
            printf("Errore nella creazione del processo %d\n", i);
            exit(1);
        } else if(pid[i] == 0){     //se sono il processo figlio calcolo, salvo il risultato ed esco
            sharedMemoryPointer->partialFattorials[i] = fatt(bounds[i]);
            exit(0);
        }
    }

    //attendo che tutti i processi finiscano
    for(i = 0; i < n && i < PROCESS_COUNT; i++){
        waitpid(pid[i], &exitCode, 0);
    }

    //calcolo del fattoriale multiprocesso
    for(i = 0; i < n && i < PROCESS_COUNT; i++){
        processFatt *= sharedMemoryPointer->partialFattorials[i];
    }

    endClock();

    //stacco la memoria condivisa dal processo
    if (shmdt(sharedMemoryPointer) == -1){
        printf("Unlink shared memory fallito!\n");
        exit(1);
    }

    //stampa del fattoriale multiprocesso
    printf("Fattoriale multiprocesso: %lld in %d clock e %d usec\n", processFatt, clocksSpent, timeSpent);

    return 0;
}

int fatt(bounds_t bounds){
    long fatt = 1;

    for(int i = bounds.from; i <= bounds.to; i++){
        fatt *= i;
    }

    return fatt;
}

long fattMono(int n){
    long fatt = 1;

    for(int i = 1; i <= n; i++){
        fatt *= i;
    }

    return fatt;
}

bounds_t * getBounds(int start, int n){
    int howMany = n < PROCESS_COUNT ? n : PROCESS_COUNT;

    bounds_t *bounds = (bounds_t *) malloc(howMany * sizeof(bounds_t));

    if(bounds){
        int i;
        int elementsPerThread = n / howMany;
        int remainder = n % howMany;

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