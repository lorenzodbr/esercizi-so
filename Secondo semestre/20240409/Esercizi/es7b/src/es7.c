#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int pid, fd[2];

    pipe(fd);

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) // ls
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        execlp("ls", "ls", "-al", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else
    {
        int fd2[2];

        pipe(fd2);

        close(fd[1]);

        if ((pid = fork()) < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // cut
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);

            dup2(fd2[1], STDOUT_FILENO);
            close(fd2[0]);
            close(fd2[1]);

            execlp("cut", "cut", "-c12-13", NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        }
        else // sort
        {
            close(fd[0]);

            close(fd2[1]);
            dup2(fd2[0], STDIN_FILENO);
            close(fd2[0]);

            execlp("sort", "sort", "-nr", NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
