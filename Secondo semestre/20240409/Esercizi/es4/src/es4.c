#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

#define SECONDS_C1 3
#define SECONDS_C2 10
#define SECONDS_P 20
#define CHILDREN 2

int pid[CHILDREN];

void parentHandler(int sig)
{
    printf("\nExiting...\n");
    kill(pid[0], SIGTERM);
    kill(pid[1], SIGTERM);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    int t;

    for (int i = 0; i < CHILDREN; i++)
    {
        if ((pid[i] = fork()) < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid[i] == 0)
        {

            if (i == 0)
            {
                while (1)
                {
                    printf("Sono figlio1, sto giocando...\n");

                    t = SECONDS_C1;
                    while (t != 0)
                        t = sleep(t);
                }
            }
            else
            {
                while (1)
                {
                    t = SECONDS_C2;
                    while (t != 0)
                        t = sleep(t);

                    printf("Stopping figlio1...\n");
                    kill(pid[0], SIGSTOP);
                }
            }
        }
    }

    signal(SIGINT, parentHandler);

    while (1)
    {
        t = SECONDS_P;
        while (t != 0)
            t = sleep(t);

        int status;

        if ((waitpid(pid[0], &status, WUNTRACED | WNOHANG)) >= 0)
        {
            if (WIFSTOPPED(status))
            {
                printf("Resuming figlio1...\n");
                kill(pid[0], SIGCONT);
            }
            else
            {
                printf("No need to resume figlio1...\n");
            }
        }
        else
        {
            printf("Error while retreiving figlio1 status\n");
        }
    }

    return 0;
}
