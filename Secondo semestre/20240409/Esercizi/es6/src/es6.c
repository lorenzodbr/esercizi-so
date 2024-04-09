#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>

#define SIZE 100

typedef struct
{
    ssize_t size;
    char value[SIZE];
} item_t;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <path1> <path2> <path3> <...>\n", *argv);
        exit(EXIT_FAILURE);
    }

    int pathsCount = argc - 1, buff[SIZE];
    char **paths = argv + 1;
    ssize_t bytesRead;
    pid_t pid;

    int fd[2];
    pipe(fd);

    for (int i = 0; i < pathsCount; i++)
    {
        if ((pid = fork()) < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // Produttore
        {
            close(fd[0]);
            int file = open(paths[i], O_RDONLY);

            printf("<Producer> adding text file: %s\n", paths[i]);

            item_t item;

            do
            {
                item.size = read(file, item.value, sizeof(item.value));
                if (item.size == -1)
                {
                    perror("read");
                    exit(EXIT_FAILURE);
                }

                if (item.size > 0)
                    write(fd[1], &item, sizeof(item.size) + item.size);
            } while (item.size > 0);

            close(file);
            close(fd[1]);

            return 0;
        }
    }

    while (wait(NULL) > 0)
        ;

    ssize_t bytesInChunk;

    close(fd[1]);
    do
    {
        if ((bytesRead = read(fd[0], &bytesInChunk, sizeof(bytesInChunk))) > 0)
        {
            printf("\n<Consumer> text file:\n");
            bytesRead = read(fd[0], buff, bytesInChunk);
            write(STDOUT_FILENO, buff, bytesRead);
        }
    } while (bytesRead > 0);

    close(fd[0]);

    return 0;
}