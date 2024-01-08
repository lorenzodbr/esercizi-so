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
// 3. utilizzando i thread. Il programma deve prendere in input 'n' e stampare i primi 'n' numeri di
//    Fibonacci. Compilare il programma utilizzando i flag appropriati (ad esempio, -pthread) a seconda
//    del vostro compilatore.

#define MAX_PROCESSES 10
#define MAX_SEQUENCE 100

typedef struct
{
    int sequence[MAX_SEQUENCE];
    int length;
} shared_data;

int main(int argc, char *argv[])
{
    int n;

    printf("Inserisci la lunghezza della sequenza di Fibonacci da stampare: ");
    scanf("%d", &n);

    return 0;
}