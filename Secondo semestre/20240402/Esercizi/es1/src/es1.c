#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 4

char * messages[] = {"Operativi.\n", "Sistemi ", " di ", "Corso"};

typedef struct {
    int i;
    int n;
    pthread_mutex_t * mutexes;    
} thread_data_t;

void errExit(char * msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

void * printMessage(void *arg){
    thread_data_t * data = (thread_data_t *) arg;

    while(data->n > 0){
        data->n--;
        
        if(pthread_mutex_lock(&data->mutexes[data->i]) != 0)
            errExit("mutex_lock");

        printf("%s", messages[data->i]);
        fflush(stdout);

        if(data->i > 0){
            if(pthread_mutex_unlock(&data->mutexes[data->i - 1]) != 0)
                errExit("mutex_unlock");
        } else {
            if(pthread_mutex_unlock(&data->mutexes[N - 1]) != 0)
                errExit("mutex_unlock");
        }
    }
    
    return NULL;
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Usage: %s N\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);

    if(n < 1){
        printf("N must be positive\n");
        exit(1);
    }

    pthread_mutex_t mutexes[N];    
    thread_data_t data[N];
    pthread_t tids[N];

    for(int i = 0; i < N; i++){
        if(pthread_mutex_init(&mutexes[i], NULL) != 0)
            errExit("mutex_init");
        
        data[i].i = i;
        data[i].mutexes = mutexes;
        data[i].n = n;

        if(i < N - 1){
            if(pthread_mutex_lock(&mutexes[i]) != 0)
                errExit("mutex_lock");
        }
    }

    for(int i = 0; i < N; i++){
        if(pthread_create(&tids[i], NULL, printMessage, &data[i]) != 0)
            errExit("pthread_create");
    }

    for(int i = 0; i < N; i++){
        if(pthread_join(tids[i], NULL) != 0)
            errExit("pthread_join");
    }

    for(int i = 0; i < N; i++){
        if(i < N - 1){
            if(pthread_mutex_unlock(&mutexes[i]) != 0)
                errExit("mutex_unlock");
        }

        if(pthread_mutex_destroy(&mutexes[i]) != 0)
            errExit("mutex_destroy");
    }

    return 0;
}
