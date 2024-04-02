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

void * printMessage(void *arg){
    thread_data_t * data = (thread_data_t *) arg;

    while(data->n > 0){
        data->n--;
        
        pthread_mutex_lock(&data->mutexes[data->i]);
        printf("%s", messages[data->i]);
        fflush(stdout);

        if(data->i > 0){
            pthread_mutex_unlock(&data->mutexes[data->i - 1]);
        } else {
            pthread_mutex_unlock(&data->mutexes[N - 1]);
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
        pthread_mutex_init(&mutexes[i], NULL);
        data[i].i = i;
        data[i].mutexes = mutexes;
        data[i].n = n;

        if(i < N - 1){
            pthread_mutex_lock(&mutexes[i]);
        }
    }

    for(int i = 0; i < N; i++){
        pthread_create(&tids[i], NULL, printMessage, &data[i]);
    }

    for(int i = 0; i < N; i++){
        pthread_join(tids[i], NULL);
    }

    for(int i = 0; i < N; i++){
        pthread_mutex_destroy(&mutexes[i]);
    }

    return 0;
}
