#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig){
    printf("Che bella dormita!\n");
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Usage: %s <seconds>\n", *argv);
        exit(EXIT_FAILURE);
    }
    
    int seconds = atoi(argv[1]);

    if(seconds <= 0){
        printf("Parameter must be positive\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGALRM, handler);
    alarm(seconds);
    pause();

    return 0;
}
