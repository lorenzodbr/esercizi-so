#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PATH_MAX 512

extern char **environ;

void getEnviron(char *, char *);
int startsWith(char *, char *);
int isSubDir(char *, char *);

// Utilizzando la variabile environ, scrivere un programma che legge lo username 
// (variabile USER) e la home (variabile HOME) dell’utente utilizzatore.
// Il programma confronta il path della sua attuale directory di lavoro con la home 
// directory dell’utente. 
// Se la directory di lavoro non e’ una sotto directory della home directory dell’utente, 
// il programma imposta la home directory dell’utente come sua directory di lavoro, 
// crea un file di testo vuoto, e stampa a video: “Caro USER, sono dentro la tua 
// home!”, dove USER e’ l’username dell’utente.
// Se la directory di lavoro e’ una sotto directory della home directory dell’utente, il 
// programma stampa a video: “Caro USER, sono gia’ nel posto giusto!”.

int main(int argc, char* argv[]){
    char *user, *home_dir, *working_dir;
    int i;
    
    working_dir = malloc(sizeof(char)*PATH_MAX);
    getcwd(working_dir, PATH_MAX);

    for(char **it = environ; (*it) != NULL && home_dir == NULL || user == NULL; it++){
        for(i = 0; *((*it) + i) != '='; i++);

        if(startsWith("HOME", *it)){
            home_dir = (*it) + strlen("HOME=");
        }

        if(startsWith("USER", *it)){
            user = (*it) + strlen("USER=");
        }
    }

    if(isSubDir(home_dir, working_dir)){
        printf("Caro %s, sono già nel posto giusto!\n", user);
    } else {
        chdir(home_dir);
        int fd = open("./nuovo_file.txt", O_CREAT);
        printf("Caro %s, sono dentro la tua home!\n", user);
    }

    return 0;
}

int isSubDir(char *sub, char *dir){
    return startsWith(sub, dir);
}

int startsWith(char *start, char *str){
    return strncmp(str, start, strlen(start)) == 0;
}