//
// Created by djdj7 on 2020-05-29.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

sig_atomic_t quitFlag;

// One signal handler for SIGINT and SIGQUIT

static void sig_int(int sigNo) {
    if (sigNo == SIGINT)
        printf("\nInterrupted by SIGINT!!!\n");
    else if (sigNo == SIGQUIT) {
        quitFlag = 1;
    }
}

int main() {
    sigset_t newMask, oldMask, zeroMask;

    // Set signal handler
    if (signal(SIGINT, sig_int) == SIG_ERR)
        printf("signal(SIGINT) error.\n");

    if (signal(SIGQUIT, sig_int) == SIG_ERR)
        printf("signal(SIGQUIT) error.\n");

    sigemptyset(&zeroMask);
    sigemptyset(&newMask);

    sigaddset(&newMask, SIGQUIT);

    // Do something to the process with a mask set
    if (sigprocmask(SIG_BLOCK, &newMask, &oldMask) < 0)
        perror("SIGBLOCK error\n");

    // Try to make a loop
    while (quitFlag == 0) {
        sigsuspend(&zeroMask);
    }

    quitFlag = 0;

    // Reset signal mask which unblocks SIGQUIT
    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) < 0)
        perror("SIG_SETMASK error\n");

    return 0;
}