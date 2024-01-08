#include <stdio.h>
#include <stdlib.h>

// Scrivere un programma C per calcolare la sequenza di Fibonacci, nelle seguenti varianti:
// 2. utilizzando processi e memoria condivisa. Il programma deve prendere
//    in input 'n' e stampare i primi 'n' numeri di Fibonacci;

int fibonacci(int);

unsigned long long *support;

int main(int argc, char *argv[])
{
    int n;

    printf("Inserisci la lunghezza della sequenza di Fibonacci da stampare: ");
    scanf("%d", &n);

    if (n <= 0)
    {
        printf("Errore: il numero deve essere maggiore di 0!\n");
        exit(1);
    }

    support = (unsigned long long *)malloc(n * sizeof(unsigned long long));

    if (!support)
    {
        printf("Errore nell'allocazione della memoria!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        support[i] = fibonacci(i);
        printf("%llu ", support[i]);
    }

    printf("\n");

    return 0;
}

int fibonacci(int i)
{
    if (i <= 1)
        return i;

    return support[i - 1] + support[i - 2];
}