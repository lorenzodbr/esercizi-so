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

//Svolgere lo stesso esercizio utilizzando memoria condivisa e processi, e non più le pthreads come 
//nel primo caso.

//Struttura per stabilire i limiti nei quali calcolare il fattoriale
typedef struct {
    int from;
    int to;
} bounds_t;

//Struttura per la memoria condivisa
struct shared {
    int partialFattorials[PROCESS_COUNT];
};

int fatt(bounds_t);
long fattMono(int);

int main(int argc, char *argv[]){
    int n;                          //numero di cui calcolare il fattoriale
    int i;                          //iteratore
    long processFatt = 1;           //fattoriale calcolato dai processi
    int exitCode;                   //codice di uscita

    pid_t pid[PROCESS_COUNT];       //array di pid per i processi
    bounds_t bounds[PROCESS_COUNT]; //array di bounds per i processi

    //creazione della memoria condivisa
    int shmId = shmget(IPC_PRIVATE, sizeof(struct shared), IPC_CREAT | S_IRUSR | S_IWUSR);
    
    //controllo della creazione della memoria condivisa
    if (shmId == -1){
        printf("Impossibile creare la shared memory!\n");
        exit(1);
    }

    //inserimento del numero di cui calcolare il fattoriale
    printf("Numero di cui calcolare il fattoriale: ");
    scanf("%d", &n);

    //calcolo del fattoriale monoprocesso
    printf("Fattoriale monoprocesso: %ld\n", fattMono(n));

    //attacco la memoria condivisa al processo
    struct shared *sharedMemoryPointer = shmat(shmId, 0, 0);

    //controllo dell'attacco della memoria condivisa
    if (sharedMemoryPointer == (void *) -1){
        printf("shmat failed");
        exit(1);
    }

    //creazione dei processi
    for(i = 0; i < n && i < PROCESS_COUNT; i++){
        //calcolo dei bounds
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

    //stacco la memoria condivisa dal processo
    if (shmdt(sharedMemoryPointer) == -1){
        printf("Unlink shared memory fallito!\n");
        exit(1);
    }

    //stampa del fattoriale multiprocesso
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

long fattMono(int n){
    long fatt = 1;

    for(int i = 1; i <= n; i++){
        fatt *= i;
    }

    return fatt;
}