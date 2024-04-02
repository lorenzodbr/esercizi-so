#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 3

typedef struct {
    int i;
    pthread_mutex_t * mutexes;    
} thread_data_t;

void * printMessage(void *arg){
    thread_data_t * data = (thread_data_t *) arg;

    pthread_mutex_lock(&data->mutexes[data->i]);
    printf("%c\n", 'A' + data->i);
    fflush(stdout);

    if(data->i < N - 1){
        pthread_mutex_unlock(&data->mutexes[data->i + 1]);
    } else {
        pthread_mutex_unlock(&data->mutexes[0]);
    }

    pthread_mutex_lock(&data->mutexes[data->i]);
    printf("%s (%c)\n", "done", 'A' + data->i);

    fflush(stdout);

    if(data->i < N - 1){
        pthread_mutex_unlock(&data->mutexes[data->i + 1]);
    }
    
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_mutex_t mutexes[N];    
    thread_data_t data[N];
    pthread_t tids[N];

    for(int i = 0; i < N; i++){
        pthread_mutex_init(&mutexes[i], NULL);
        data[i].i = i;
        data[i].mutexes = mutexes;

        if(i > 0){
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
