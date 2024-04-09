#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 100

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <path>\n", *argv);
        exit(EXIT_FAILURE);
    }

    char *path = argv[1];

    int fd[2];
    pipe(fd);

    int buff[SIZE], bytesRead, pid, whereToRead, whereToWrite;

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    { // Produttore
        close(fd[0]);

        whereToWrite = fd[1];
        whereToRead = open(path, O_RDONLY);

        if (whereToRead < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
    }
    else
    { // Consumatore
        close(fd[1]);

        whereToRead = fd[0];
        whereToWrite = STDOUT_FILENO;
    }

    while ((bytesRead = read(whereToRead, buff, sizeof(buff))) > 0)
    {
        if (write(whereToWrite, buff, bytesRead) < 0)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (bytesRead < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    return 0;
}
