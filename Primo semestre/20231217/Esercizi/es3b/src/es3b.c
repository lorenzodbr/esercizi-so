#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

// Scrivere un programma C per calcolare la sequenza di Fibonacci, nelle seguenti varianti:
// 2. utilizzando processi e memoria condivisa. Il programma deve prendere
//    in input 'n' e stampare i primi 'n' numeri di Fibonacci;

#define MAX_PROCESSES 10
#define MAX_SEQUENCE 100

typedef struct
{
    int start;
    int end;
    unsigned long long *sequence;
    int sem_id;
    int process_num;
} shared_data;

unsigned long long fibonacci(int n);
void calculateFibonacci(shared_data *data);

int main(int argc, char *argv[])
{
    int seqLength, numProcesses;

    do
    {
        printf("Inserisci il numero di processi da utilizzare: ");
        scanf("%d", &numProcesses);
    } while (numProcesses < 1 || numProcesses > MAX_PROCESSES);

    do
    {
        printf("Inserisci la lunghezza della sequenza di Fibonacci da stampare: ");
        scanf("%d", &seqLength);
    } while (seqLength < 1 || seqLength > MAX_SEQUENCE);

    int shm_id = shmget(IPC_PRIVATE, sizeof(unsigned long long) * seqLength, IPC_CREAT | 0666);
    unsigned long long *result = (unsigned long long *)shmat(shm_id, NULL, 0);

    // 1 = valore di inizializzazione del semaforo
    int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    semctl(sem_id, 0, SETVAL, 1);

    pid_t pids[MAX_PROCESSES];
    shared_data *data = (shared_data *) malloc(sizeof(shared_data) * numProcesses);

    int processLength = seqLength / numProcesses;
    int remaining = seqLength % numProcesses;

    for (int i = 0; i < numProcesses; i++)
    {
        data[i].start = i * processLength;
        data[i].end = data[i].start + processLength - 1;
        data[i].sequence = result;
        data[i].sem_id = sem_id;
        data[i].process_num = i;

        if (remaining > 0)
        {
            data[i].end++;
            remaining--;
        }

        pids[i] = fork();

        if (pids[i] == 0)
        {
            calculateFibonacci(&data[i]);
        }
        else if (pids[i] < 0)
        {
            printf("Errore durante la creazione del processo %d\n", i);
            exit(1);
        }
    }

    for (int i = 0; i < numProcesses; i++)
    {
        waitpid(pids[i], NULL, 0);
    }

    shmdt(result);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    printf("La sequenza di Fibonacci di lunghezza %d è:\n", seqLength);

    for (int i = 0; i < seqLength; i++)
    {
        printf("%llu ", result[i]);
    }

    printf("\n");

    free(data);
    return 0;
}

unsigned long long fibonacci(int n)
{
    if (n <= 1)
        return n;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}

void calculateFibonacci(shared_data *data)
{
    for (int i = data->start; i <= data->end; i++)
    {
        unsigned long long fib = fibonacci(i);

        // 0 = sem id,
        //-1 = operazione di wait (decremento),
        // 0 = flag addizionali
        struct sembuf sb = {0, -1, 0};
        semop(data->sem_id, &sb, 1);

        data->sequence[i] = fib;

        // 0 = sem id,
        // 1 = operazione di signal (incremento),
        // 0 = flag addizionali
        sb.sem_op = 1;
        semop(data->sem_id, &sb, 1);
    }

    exit(0);
}