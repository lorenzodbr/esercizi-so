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

#define N 4

#ifndef SEMUN_H
#define SEMUN_H
// definition of the union semun
union semun {
    int val;
    struct semid_ds * buf;
    unsigned short * array;
};
#endif

void disposeSem(int semid){
    if(semctl(semid, 0, IPC_RMID, NULL)){
        perror("<semctl>");
        exit(1);
    }
}

void P(int semid, int index){
    struct sembuf sb = {index, -1, 0};
    if(semop(semid, &sb, 1) < 0){
        perror("<P>");
        disposeSem(semid);
    }
}

void V(int semid, int index){
    struct sembuf sb = {index, 1, 0};
    if(semop(semid, &sb, 1) < 0){
        perror("<V>");
        disposeSem(semid);
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Usage: %s N", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);

    if(n < 1){
        printf("N must be a positive integer");
        exit(1);
    }
    
    char *prints[] = {"Operativi.\n", "Sistemi ", " di ", "Corso"};

    int pid;

    int semid = semget(IPC_PRIVATE, N, IPC_CREAT | 0640); 

    if(semid < 0){
        perror("<semget>");
    }

    short unsigned values[] = {0, 0, 0, 1}; 
    union semun arg;
    arg.array = values;
    
    if(semctl(semid, 0, SETALL, arg) < 0){
        perror("<semctl>");
        disposeSem(semid);
        exit(1);
    }

    for(int i = 0; i < N; i++){
        pid = fork();

        if(pid < 0){
            printf("<fork>");
            disposeSem(semid);
            exit(1);
        } else if(pid == 0){
            for(int j = 0; j < n; j++){
                P(semid, i);

                printf("%s", prints[i]);
                fflush(stdout);

                if(i > 0){
                    V(semid, i - 1);
                } else {
                    V(semid, N - 1);
                }
            }
            
            return 0;
        }
    }

    while(wait(NULL) != -1);

    disposeSem(semid);
    return 0;
}
