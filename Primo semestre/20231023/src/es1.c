#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_LEN 128

// Utilizzando solamente le system call open e read, e la funzione printf, realizzare un 
// programma che simuli il comportamento del comando cat. Utilizzo: my_cat <file1> ... <filen>

int main(int argc, char const *argv[])
{
    int i, fd;
    ssize_t bytesRead;
    char *buffer;

    buffer = (char *) malloc((BUF_LEN + 1)*sizeof(char));

    if(!buffer){
        printf("Errore nell'allocazione della memoria");
        return -1;
    }

    if(argc == 1) {
        printf("Utilizzo: my_cat <file1> <file2> ... <fileN>\n");
        return -1;
    }

    for(i = 1; i < argc; i++){
        fd = open(argv[i], O_RDONLY);

        if(fd != -1){
            do {
                bytesRead = read(fd, buffer, BUF_LEN);

                if(bytesRead > 0){
                    buffer[BUF_LEN] = '\0';
                    printf("%s\n", buffer);
                }

            } while(bytesRead > 0);

            close(fd);
        } else {
            printf("Impossibile aprire: %s\n", argv[i]);
        }
    }

    return 0;
}
