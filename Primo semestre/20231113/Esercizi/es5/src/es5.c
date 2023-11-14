#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define COMMAND 1

// Scrivere un programma che legge della riga di comando un comando di sistema X e 
// i suoi argomenti.
// Il programma crea un processo figlio, il quale, utilizzando la system call exec, 
// esegue X reindirizzando il suo standard output ed error su un file di testo. Il 
// processo parent attende la terminazione del processo figlio, stampa a video il codice
// di terminazione, ed infine termina.

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("usage: es5 <command> [arg1, arg2, ...]");
        return -1;
    }

    pid_t pid = fork();

    if(pid == 0){           //figlio
        int stdout_fd = open("stdout.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
        int stderr_fd = open("stderr.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

        close(STDOUT_FILENO);
        dup(stdout_fd);
        close(STDERR_FILENO);
        dup(stderr_fd);

        execvp(*(argv + COMMAND), argv + COMMAND);

        return -1;          //eseguita solo in caso di errore
    } else if(pid == -1){   //errore
        return -1;
    } else {                //padre
        int status;
        
        while((pid = wait(&status)) != -1){
            printf("Codice di terminazione di %d: %d\n", pid, status);
        }
    }

    return 0;
}
