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

#define N 3


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

void printSemStatus(int semid){
    short unsigned values[N];
    union semun arg;
    arg.array = values;
    
    if (semctl(semid, 0, GETALL, arg) == -1){
        perror("<semctl>");
        disposeSem(semid);
        exit(1);
    }

    printf("Semaphore %d\n---------------\nIndexes:  ", semid);
    for(int i = 0; i < N; i++){
        printf("%d ", i);
    }

    printf("\nValues:   ");

    for(int i = 0; i < N; i++){
        printf("%d ", values[i]);
    }
    printf("\n");
    fflush(stdout);
}

int main(int argc, char *argv[]){
    int semid = semget(IPC_PRIVATE, N, IPC_CREAT | 0640); 

    if(semid < 0){
        perror("<semget>");
    }

    short unsigned values[] = {0, 0, 1}; 
    union semun arg;
    arg.array = values;

    if(semctl(semid, 0, SETALL, arg) < 0){
        perror("<semctl>");
        disposeSem(semid);
        exit(1);
    }

    printSemStatus(semid);
    printf("\n");
    
    for(int i = 0; i < N; i++){
        int pid = fork();

        if(pid < 0){
            perror("fork");
        } else if (pid == 0) {
            P(semid, i);
            
            printf("%c\n", 'C' - i);

            V(semid, i == 0 ? N - 1 : i - 1);

            P(semid, i);
            
            printf("done\n");

            if(i > 0){
                V(semid, i - 1);
            }

            return 0;
        }
    }

    while(wait(NULL) != -1);

    printf("\n");
    printSemStatus(semid);

    disposeSem(semid);
    return 0;
}
