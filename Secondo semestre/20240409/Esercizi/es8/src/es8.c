#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PING "ping"
#define PONG "pong"

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
    int pid, fdAM[2], fdMA[2], n, bR, bW;
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
            bW = write(fdAM[WRITE], PING, sizeof(PING));

            if(bW == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }

            bR = read(fdMA[READ], buff, sizeof(PONG));

            if(bR == -1 || bR != sizeof(PONG))
            {
                perror("read");
                exit(EXIT_FAILURE);
            }

            printf("\t%s\n", buff);
        }
    }
    else
    {
        close(fdMA[READ]);
        close(fdAM[WRITE]);

        for (int i = 0; i < n; i++)
        {
            bR = read(fdAM[READ], buff, sizeof(PING));

            if(bR == -1 || bR != sizeof(PING))
            {
                perror("read");
                exit(EXIT_FAILURE);
            }

            printf("%d)\t%s\n", i + 1, buff);
            bW = write(fdMA[WRITE], PONG, sizeof(PONG));

            if(bW == -1 || bW != sizeof(PONG))
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
