#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define USAGE "Utilizzo: search_replace <source file> <destination file> <char search> <char replace>\n"
#define SOURCE_FILE 1
#define DESTINATION_FILE 2
#define CHAR_SEARCH 3
#define CHAR_REPLACE 4

// Realizzare un programma che sfruttando le system call per la gestione dei file 
// esegua un search_replace di un carattere all’interno di un file.
// search_replace <source file> <destination file> <char search> <char 
// replace>
//  Il programma copia il contenuto del file <source file> in un nuovo file di nome 
// <destination file>.  Dovrà sostituire tutte le occorrenze di <char search> con <char
// replace>

int main(int argc, char const *argv[])
{
    int i, j, k, fdIn, fdOut;
    ssize_t bytesRead, bytesWritten;
    off_t offset;
    char charBuf;

    if(argc < 5) {
        printf(USAGE);
        return -1;
    }

    for(i = 0; argv[CHAR_SEARCH][i] != '\0'; i++);
    for(j = 0; argv[CHAR_REPLACE][j] != '\0'; j++);

    if(i != 1 || j != 1){
        printf("%d, %d, %s", i, j, USAGE);
        return -1;
    }

    fdIn = open(argv[SOURCE_FILE], O_RDONLY);
    fdOut = open(argv[DESTINATION_FILE], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if(fdIn != -1){
        do {
            bytesRead = read(fdIn, &charBuf, sizeof(charBuf));

            if(bytesRead > 0){
                if(charBuf == argv[CHAR_SEARCH][0]){
                    charBuf = argv[CHAR_REPLACE][0];
                }

                bytesWritten = write(fdOut, &charBuf, sizeof(charBuf));
            }
        } while (bytesRead > 0);

        close(fdIn);
        close(fdOut);
    } else {
        printf("Errore nell'apertura del file");
        return -1;
    }

    return 0;
}