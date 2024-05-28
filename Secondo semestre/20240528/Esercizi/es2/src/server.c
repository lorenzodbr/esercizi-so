#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <unistd.h>

#include "../inc/order.h"
#include "../inc/errExit.h"

// the message queue identifier
int msqid = -1;

void signTermHandler(int sig) {
    // do we have a valid message queue identifier
    if (msqid > 0) {
        if (msgctl(msqid, IPC_RMID, NULL) == -1)
            errExit("msgctl failed");
        else
            printf("<Server> message queue removed successfully\n");
    }

    // terminate the server process
    exit(0);
}

int main (int argc, char *argv[]) {
    // check command line input arguments
    if (argc != 2) {
        printf("Usage: %s message_queue_key\n", argv[0]);
        exit(1);
    }

    // read the message queue key defined by user
    key_t key = atoi(argv[1]);

    if (key <= 0) {
        errExit("key must be > 0");
    }

    sigset_t set;
    // initialize mySet to contain all signals
    sigfillset(&set);
    // remove SIGTERM from mySet
    sigdelset(&set, SIGTERM);
    // blocking all signals but SIGTERM
    sigprocmask(SIG_BLOCK, &set, NULL);

    // set the function sigHandler as handler for the signal SIGTERM
    signal(SIGTERM, signTermHandler);

    printf("<Server> Making MSG queue...\n");
    // get the message queue, or create a new one if it does not exist
    if ((msqid = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        errExit("msgget");
    }

    // check functionality
    printf("<Server> sleep...\n");
    sleep(30);
    // the process sleeps for 1 minutes. Try to send some orders
    // and check that prime users' orders are always read before
    // normal users' ones
    printf("<Server> ready...\n");

    struct order order;
    // endless loop
    while (1) {
	// read a message from the message queue.
        // type is set equal to -2. Thus, first all messages with type 1 are read.
        // When no message with type 1 is in the queue, then messages with type
        // 2 are read from the queue.
        size_t mSize = sizeof(struct order) - sizeof(long);
        msgrcv(msqid, &order, mSize, -2, 0);

        // print the order on standard output
        printOrder(&order);
    }

    return 0;
}
