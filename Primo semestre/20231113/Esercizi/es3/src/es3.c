#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_RAND 256

// Modificare il programma dell’ “es2” affinche’ il processo parent attenda solo la 
// terminazione dell’ultimo figlio creato. L’attesa deve avvenire in modalità polling!

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
        } else if(pid == -1){
            return -1;
        } else {
            pid = getpid();
            ppid = getppid();
            srand(pid);
            printf("(F) Sono un figlio con pid = %d, mio padre ha pid = %d\n", pid, ppid);
            exit(rand() % MAX_RAND);
        }
    }

    while((pid = waitpid(pid, &status, WUNTRACED)) != -1){
        if(WIFEXITED(status)){
            printf("(P) L'ultimo figlio con pid = %d ha terminato con stato = %d\n", pid, WEXITSTATUS(status));
        }
    }
    
    return 0;
}
