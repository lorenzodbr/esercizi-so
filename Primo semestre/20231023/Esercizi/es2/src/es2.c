#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

// Utilizzando solamente le system call open, read, lseek, e write, realizzare un 
// programma che simuli il comportamento del comando tac. Al contrario di cat, il 
// comando tac stampa il contenuto di un file al contrario (dall’ultimo carattere al 
// primo)
// Utilizzo: my_tac <file1> ... <filen>

int main(int argc, char const *argv[])
{
    int i, j, k, fd;
    ssize_t bytesRead, bytesWritten;
    off_t offset;
    char charBuf;

    if(argc == 1) {
        printf("Utilizzo: my_tac <file1> ... <filen>\n");
        return -1;
    }

    for(i = 1; i < argc; i++){
        fd = open(argv[i], O_RDONLY);

        if(fd != -1){
            offset = lseek(fd, -1, SEEK_END);
            
            while(offset >= 0){
                bytesRead = read(fd, &charBuf, sizeof(charBuf));

                if(bytesRead > 0){
                    bytesWritten = write(STDOUT_FILENO, &charBuf, sizeof(charBuf));

                    if (bytesWritten != sizeof(charBuf)){
                        printf("Errore nella stampa\n");
                        return -1;
                    }
                }
                
                offset = lseek(fd, -2, SEEK_CUR);
            }

            close(fd);

            charBuf = '\n';
            bytesWritten = write(STDOUT_FILENO, &charBuf, sizeof(charBuf));

            if (bytesWritten != sizeof(charBuf)){
                printf("Errore nella stampa\n");
                return -1;
            }
        } else {
            printf("Impossibile aprire: %s\n", argv[i]);
        }
    }

    return 0;
}
