#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

#define CHILDREN 2

int pid[CHILDREN];

void child1Handler(int sig)
{
    printf("<figlio1> SIGUSR2 ricevuto!\n");
}

void child2Handler(int sig)
{
    printf("<figlio2> SIGUSR1 ricevuto!\n");
}

void parent1Handler(int sig)
{
    kill(pid[1], SIGUSR1);
}

void parent2Handler(int sig)
{
    kill(pid[0], SIGUSR2);
}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, parent1Handler);
    signal(SIGUSR2, parent2Handler);

    for (int i = 0; i < CHILDREN; i++)
    {
        if ((pid[i] = fork()) < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid[i] == 0)
        {
            int ppid = getppid();

            if (i == 0)
            {
                signal(SIGUSR2, child1Handler);
                kill(ppid, SIGUSR1);
                pause();
            }
            else
            {
                signal(SIGUSR1, child2Handler);
                pause();
                kill(ppid, SIGUSR2);
            }

            return 0;
        }
    }

    pause();
    pause();

    while (wait(NULL) != -1)
        ;

    return 0;
}
