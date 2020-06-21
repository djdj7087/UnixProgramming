//
// Created by wooyoung on 5/15/20.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

static void sig_quit(int sig_num) {

    printf("Catch SIGQUIT\n");

    /* Reset SIGQUIT to the default action */
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
        perror("Cannot reset SIGQUIT: \n");

    return;
}

int main(void) {

    sigset_t new_mask, old_mask, pend_mask;

    /* Set signal handler */
    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        perror("Cannot catch SIGQUIT: \n");

    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGQUIT);
    //sigaddset(&new_mask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        perror("SIG_BLOCK ERROR: \n");
    }

    sleep(5);

    if (sigpending(&pend_mask) < 0)
        perror("sigpending error: \n");

    if (sigismember(&pend_mask, SIGQUIT) == 1)
        printf("SIGQUIT is pending\n");

    /* After 5 seconds */
    /* Reset Signal mask which unblocks SIGQUIT */
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
        perror("SIG_SETMASK ERROR: \n");

    /* We knew */
    printf("SIGQUIT is unblocked \n");

    sleep(5);

    /* SIGQUIT here will terminate with core file */

//    for (;;);

    return 0;
}