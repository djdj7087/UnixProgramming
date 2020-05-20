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
    pr_mask("\nIn sig_int: \n");
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
        perror("sigprocmask error: \n");

    if (sigismember(&sig_set, SIGINT)) printf("SIGINT\n");
    if (sigismember(&sig_set, SIGQUIT)) printf("SIGQUIT\n");
    if (sigismember(&sig_set, SIGUSR1)) printf("SIGUSR1\n");
    if (sigismember(&sig_set, SIGALRM)) printf("SIGALRM\n");
}

int main(void) {

    sigset_t new_mask, old_mask, wait_mask;

    pr_mask("Program Start: \n");

    if (signal(SIGINT, sig_int) == SIG_ERR)
        perror("SIGINT ERROR: \n");

    sigemptyset(&wait_mask);

    return 0;
}