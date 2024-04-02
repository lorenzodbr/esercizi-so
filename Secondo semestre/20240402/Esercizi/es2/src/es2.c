#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define N 3

typedef struct {
    int i;
    pthread_mutex_t * mutexes;    
} thread_data_t;

void errExit(char * msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

void * printMessage(void *arg){
    thread_data_t * data = (thread_data_t *) arg;

    if(pthread_mutex_lock(&data->mutexes[data->i]) != 0)
        errExit("mutex_lock");

    printf("%c\n", 'A' + data->i);
    fflush(stdout);

    if(pthread_mutex_unlock(&data->mutexes[(data->i + 1) % N]) != 0)
        errExit("mutex_unlock");

    if(pthread_mutex_lock(&data->mutexes[data->i]) != 0)
        errExit("mutex_lock");
        
    printf("%s (%c)\n", "done", 'A' + data->i);
    fflush(stdout);

    if(pthread_mutex_unlock(&data->mutexes[(data->i + 1) % N]) != 0)
        errExit("mutex_unlock");
    
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_mutex_t mutexes[N];    
    thread_data_t data[N];
    pthread_t tids[N];

    for(int i = 0; i < N; i++){
        if(pthread_mutex_init(&mutexes[i], NULL) != 0)
            errExit("mutex_init");

        data[i].i = i;
        data[i].mutexes = mutexes;

        if(i > 0 && pthread_mutex_lock(&mutexes[i]) != 0)
            errExit("mutex_lock");
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
        if(i > 0 && pthread_mutex_unlock(&mutexes[i]) != 0)
            errExit("mutex_unlock");

        if(pthread_mutex_destroy(&mutexes[i]) != 0)
            errExit("mutex_destroy");
    }

    return 0;
}
