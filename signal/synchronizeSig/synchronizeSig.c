//
// Created by djdj7 on 2020-05-29.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

/* Set nonzero by sig handler */
static volatile sig_atomic_t sigFlag;
static sigset_t newMask, oldMask, zeroMask;

/* One signal handler for SIGUSR1 and SIGUSR2 */
static void sig_usr(int sigNo) {
    sigFlag = 1;
}

static void char_at_a_time(char *str) {
    char *ptr;
    int c;

    /*
     * Set unbuffered
     * When we make stdout (standard output -> monitor) unbuffered,
     * parent and child process can cut in the stdout easily
     */
    setbuf(stdout, NULL);

    /*
     * c != 0 means not null zero
     * string ends with null zero
     */
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);
}

void TELL_WAIT(void) {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        perror("signal(SIGUSR1) error");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        perror("signal(SIGUSR2) error");

    /* Initialize mask set */
    sigemptyset(&zeroMask);
    sigemptyset(&newMask);
    sigaddset(&newMask, SIGUSR1);
    sigaddset(&newMask, SIGUSR2);

    /* Block SIGUSR1 and SIGUSR2, and save current signal mask */
    if (sigprocmask(SIG_BLOCK, &newMask, &oldMask) < 0)
        perror("SIG_BLOCK error");

}

void TELL_PARENT(pid_t pid) {
    kill(pid, SIGUSR2);
}

void WAIT_PARENT(void) {
    while (sigFlag == 0)
        sigsuspend(&zeroMask);

    /* Back to the previous signal mask (SIG_BLOCK with SIGUSR2 and SIGUSR1) */
    /*
     * Still blocking SIGUSR1 and SIGUSR2 when we access sigFlag....
     * Why? - sigFlag is shared by parent and child process and sigFlag can be
     * changed at any time by signals SIGUSR1 and SIGUSR2...
     */
    sigFlag = 0;

    /*
     * After we successfully change the value of sigFlag into 0,
     * Now we need to allow SIGUSR1 and SIGUSR2 for the next synchronization
     * Parent and Child processes can receive the SIGUSR1 & SIGUSR2 from each other
     *
     * That's why Reset signal mask to original value
     *
     * We need to allow sending signal SIGUSR1 and SIGUSR2...
     * Why? - When we are finished with critical
     *
     * If you do not allow further SIGUSR1 and SIGUSR2...
     * May be getting into the deadlock situation...
     */
    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) < 0)
        perror("SIG_SETMASK error");
}

void TELL_CHILD(pid_t pid) {
    kill(pid, SIGUSR1);
}

void WAIT_CHILD(void) {
    while (sigFlag == 0)
        sigsuspend(&zeroMask);

    /* Back to the previous signal mask (SIG_BLOCK with SIGUSR2 and SIGUSR1) */
    /*
     * Still blocking SIGUSR1 and SIGUSR2 when we access sigFlag....
     * Why? - sigFlag is shared by parent and child process and sigFlag can be
     * changed at any time by signals SIGUSR1 and SIGUSR2...
     */
    sigFlag = 0;

    /*
     * After we successfully change the value of sigFlag into 0,
     * Now we need to allow SIGUSR1 and SIGUSR2 for the next synchronization
     * Parent and Child processes can receive the SIGUSR1 & SIGUSR2 from each other
     *
     * That's why Reset signal mask to original value
     *
     * We need to allow sending signal SIGUSR1 and SIGUSR2...
     * Why? - When we are finished with critical
     *
     * If you do not allow further SIGUSR1 and SIGUSR2...
     * May be getting into the deadlock situation...
     */
    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) < 0)
        perror("SIG_SETMASK error");
}

int main(void) {
    pid_t pid;
    char str[100];

    /* Initialize signal mask and handler */
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        perror("fork error");
    } else if (pid == 0) {
        // Child
        for (int i = 0; i < 10; ++i) {
            WAIT_PARENT();

            char_at_a_time("Output from Child: ");
            sprintf(str, "%d", i);
            char_at_a_time(str);
            char_at_a_time("\n");

            TELL_PARENT(getpid());
        }
        exit(0);
    } else {
        // Parent
        for (int i = 0; i < 10; ++i) {
            char_at_a_time("Output from Parent: ");
            sprintf(str, "%d", i);
            char_at_a_time(str);
            char_at_a_time("\n");

            TELL_CHILD(pid);
            WAIT_CHILD();
        }
    }

    return 0;
}