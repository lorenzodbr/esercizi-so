#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define SECONDS 3

void handler(int sig){
    printf("\nCtrl+C non mi può fermare!\n");
}

int main(int argc, char *argv[]){
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGINT);
    sigprocmask(SIG_SETMASK, &set, NULL);

    if(signal(SIGINT, handler) < 0){
        perror("signal");
        exit(EXIT_FAILURE);
    }

    while(1){
        printf("Ciao, io sono un processo in esecuzione (%d)\n", getpid());
        
        int t = SECONDS;
        while(t != 0)
            t = sleep(t);
    }
    
    return 0;
}
