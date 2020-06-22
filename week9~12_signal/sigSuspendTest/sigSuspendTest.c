//
// Created by wooyoung on 5/19/20.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

void sig_int(int sig_no);

void pr_mask(const char *str);

void sig_int(int sig_no) {
    pr_mask("\nIn sig_int: ");
}

void pr_mask(const char *str) {
    sigset_t sig_set;

    /*
     * Without this code, sigset is located on the stack.
     * The sigset has some garbage values.
     * That's why we need to initialize 0s for all signals
     * So we do sigprocmask(0, NULL....)
     */
    if (sigprocmask(0, NULL, &sig_set) < 0)
        perror("sigprocmask error: ");

    printf("%s", str);

    if (sigismember(&sig_set, SIGINT)) printf("SIGINT\n");
    if (sigismember(&sig_set, SIGQUIT)) printf("SIGQUIT\n");
    if (sigismember(&sig_set, SIGUSR1)) printf("SIGUSR1\n");
    if (sigismember(&sig_set, SIGALRM)) printf("SIGALRM\n");

    printf("\n");
}

int main(void) {

    sigset_t new_mask, old_mask, wait_mask;

    pr_mask("Program Start: ");

    if (signal(SIGINT, sig_int) == SIG_ERR)
        perror("SIGINT ERROR: ");

    pr_mask("After calling sig_int: ");

    sigemptyset(&wait_mask);
    sigaddset(&wait_mask, SIGUSR1);

    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGINT);

    /* Block SIGINT (Ctrl + C) and save current signal mask (old_mask) */
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0)
        perror("SIG_BLOCK ERROR: ");

    pr_mask("int critical region: ");

    /* Pause, allowing all signals except SIGUSR1 */
    if (sigsuspend(&wait_mask) != -1)
        perror("sigsuspend ERROR: ");

    pr_mask("After return from suspend: ");

    /* Reset signal mask which unblocks SIGINT */
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
        perror("SIG_SETMASK ERROR: ");

    pr_mask("Program Exit...: ");

    return 0;
}