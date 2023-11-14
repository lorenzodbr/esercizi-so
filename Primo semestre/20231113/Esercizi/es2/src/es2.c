#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// Scrivere un programma che legge da riga di comando un intero positivo N.
// Il programma successivamente crea N processi figlio. Ogni processo figlio, stampa a
// video il suo PID, il PID del suo processo padre, ed infine termina con un codice di 
// terminazione (un numero casuale tre 0 e 255). Dopo avere creato N processi figlio, il
// programma attende la terminazione di ogni processo figlio. Ogni qualvolta un 
// processo figlio termina, il programma stampa a video il codice di terminazione del 
// figlio.

int main(int argc, char* argv[]){
    int n, r, status;
    pid_t pid;
    pid_t ppid; 
    do {
        printf("Inserisci un intero n positivo: ");
        scanf("%d", &n);
    } while(n < 0);
    
    while(n > 0){
        pid = fork();
        if(pid > 0){
            n--;
        } else {
            pid = getpid();
            ppid = getppid();
            srand(pid);
            printf("(F) Sono un figlio con pid = %d, mio padre ha pid = %d\n", pid, ppid);
            exit(rand() % 256);
        }
    }

    while((pid = wait(&status)) != -1){
        if(WIFEXITED(status)){
            printf("(P) Il figlio con pid = %d ha terminato con stato = %d\n", pid, WEXITSTATUS(status));
        }
    }
    
    return 0;
}
