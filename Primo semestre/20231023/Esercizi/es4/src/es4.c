#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define USAGE "Utilizzo: my_cp -[sa] <source file> <destination file> <char search> <char replace>\n"
#define SOURCE_FILE 2
#define DESTINATION_FILE 3

#define BUF_LEN 128

int overwrite(const char *source_file, const char *destination_file);
int do_not_overwrite(const char *source_file, const char *destination_file);
int append(const char *source_file, const char *destination_file);
int copy(int fdFrom, int fdTo);

// Realizzare un programma che sfruttando le system call per la gestione dei file 
// esegua un search_replace di un carattere all’interno di un file.
// search_replace <source file> <destination file> <char search> <char 
// replace>
//  Il programma copia il contenuto del file <source file> in un nuovo file di nome 
// <destination file>.  Dovrà sostituire tutte le occorrenze di <char search> con <char
// replace>

int main(int argc, char * const argv[]) {
    int opt;

    enum {DO_NOT_OVERWRITE, OVERWRITE, CHECK_EXISTENCE} mode = DO_NOT_OVERWRITE;

    if(argc < 3){
        printf(USAGE);
        return -1;
    }

    while ((opt = getopt(argc, argv, "sa")) != -1) {
        switch (opt) {
            case 's': mode = OVERWRITE; 
                  break;
            case 'a': mode = CHECK_EXISTENCE; 
                  break;
            default: printf(USAGE); return -1;
        }
    }

    switch (mode) {
        case OVERWRITE: return overwrite(argv[SOURCE_FILE], argv[DESTINATION_FILE]);
        case CHECK_EXISTENCE: return append(argv[SOURCE_FILE], argv[DESTINATION_FILE]);
        default: return do_not_overwrite(argv[SOURCE_FILE - 1], argv[DESTINATION_FILE - 1]);
    }
}

int overwrite(const char *source_file, const char *destination_file){
    int fdIn, fdOut;

    fdIn = open(source_file, O_RDONLY);
    fdOut = open(destination_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

    return copy(fdIn, fdOut);
}

int append(const char *source_file, const char *destination_file){
    int fdIn, fdOut;

    if(access(destination_file, F_OK) != 0){
        printf("Il file %s non esiste!\n", destination_file);
        return -1;
    }
    
    fdOut = open(destination_file, O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    fdIn = open(source_file, O_RDONLY);
        
    return copy(fdIn, fdOut);
}

int do_not_overwrite(const char *source_file, const char *destination_file){
    int fdIn, fdOut;

    if(access(destination_file, F_OK) == 0){
        printf("Il file %s già esiste!\n", destination_file);
        return -1;
    }

    fdOut = open(destination_file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);       
    fdIn = open(source_file, O_RDONLY);

    return copy(fdIn, fdOut);
}

int copy(int fdIn, int fdOut){
    ssize_t bytesRead, bytesWritten;

    char *buffer;

    if(fdIn == -1 || fdOut == -1){
        if(fdIn != -1) close(fdIn);
        if(fdOut != -1) close(fdOut);
            
        printf("Errore nell'apertura dei file!\n");

        return -1; 
    }

    buffer = (char *) calloc((BUF_LEN + 1), sizeof(char));
        
    do {
        bytesRead = read(fdIn, buffer, sizeof(buffer));

        if(bytesRead > 0){
            bytesWritten = write(fdOut, buffer, bytesRead);

            if (bytesWritten != bytesRead){
                printf("Errore nella scrittura del file\n");
                
                close(fdIn);
                close(fdOut);
                free(buffer);
                
                return -1;
            }
        }
    } while (bytesRead > 0);

    close(fdIn);
    close(fdOut);
    free(buffer);

    return 0;
}