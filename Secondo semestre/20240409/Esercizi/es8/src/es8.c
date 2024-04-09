#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PING "ping"
#define PONG "pong"

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
    int pid, fdAM[2], fdMA[2], n, bR;
    char buff[sizeof(PING) + 1];

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    pipe(fdAM);
    pipe(fdMA);

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        close(fdAM[READ]);
        close(fdMA[WRITE]);

        for (int i = 0; i < n; i++)
        {
            write(fdAM[WRITE], PING, 4);
            bR = read(fdMA[READ], buff, 4);
            printf("\t%s\n", buff);
        }
    }
    else
    {
        close(fdMA[READ]);
        close(fdAM[WRITE]);

        for (int i = 0; i < n; i++)
        {
            bR = read(fdAM[READ], buff, 4);
            buff[bR] = '\0';
            printf("%d)\t%s\n", i + 1, buff);
            write(fdMA[WRITE], PONG, 4);
        }
    }

    return 0;
}
