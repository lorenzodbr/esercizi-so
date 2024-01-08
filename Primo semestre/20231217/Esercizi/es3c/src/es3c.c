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
#include <pthread.h>
#include <sys/time.h>

// Scrivere un programma C per calcolare la sequenza di Fibonacci, nelle seguenti varianti:
// 3. utilizzando i thread. Il programma deve prendere in input 'n' e stampare i primi 'n' numeri di
//    Fibonacci. Compilare il programma utilizzando i flag appropriati (ad esempio, -pthread) a seconda
//    del vostro compilatore.

#define MAX_THREADS 10
#define MAX_SEQUENCE 100

typedef struct
{
    int start;
    int end;
    pthread_mutex_t *mutex;
    unsigned long long *result;
    int thread_num;
} shared_data;

void *calculateFibonacci(void *arg);
unsigned long long fibonacci(int n);

int main(int argc, char *argv[])
{
    int seqLength, numThreads;

    do
    {
        printf("Inserisci il numero di thread da utilizzare: ");
        scanf("%d", &numThreads);
    } while (numThreads < 1 || numThreads > MAX_THREADS);

    do
    {
        printf("Inserisci la lunghezza della sequenza di Fibonacci da stampare: ");
        scanf("%d", &seqLength);
    } while (seqLength < 1 || seqLength > MAX_SEQUENCE);

    unsigned long long *result = (unsigned long long *)malloc(sizeof(unsigned long long) * seqLength);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    pthread_t tids[MAX_THREADS];
    shared_data *data = (shared_data *)malloc(sizeof(shared_data) * numThreads);

    int threadLength = seqLength / numThreads;
    int remaining = seqLength % numThreads;

    for (int i = 0; i < numThreads; i++)
    {
        data[i].start = i * threadLength;
        data[i].end = (i + 1) * threadLength - 1;
        data[i].result = result;
        data[i].mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        data[i].thread_num = i + 1;

        if (remaining > 0)
        {
            data[i].end++;
            remaining--;
        }

        pthread_create(&tids[i], NULL, calculateFibonacci, (void *)&data[i]);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(tids[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    
    printf("La sequenza di Fibonacci di lunghezza %d è:\n", seqLength);
    
    for (int i = 0; i < seqLength; i++)
    {
        printf("%llu ", result[i]);
    }

    free(result);
    free(data);

    printf("\n");

    return 0;
}

void *calculateFibonacci(void *arg)
{
    shared_data *data = (shared_data *)arg;

    for (int i = data->start; i <= data->end; i++)
    {
        unsigned long long fib = fibonacci(i);

        pthread_mutex_lock(data->mutex);
        data->result[i] = fib;

        printf("Thread %d: %llu\n", data->thread_num, fib);

        pthread_mutex_unlock(data->mutex);
    }

    pthread_exit(NULL);
}

unsigned long long fibonacci(int n)
{
    if (n <= 1)
        return n;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}