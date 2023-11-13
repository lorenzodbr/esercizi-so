#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

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
