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
    if (signal(SIGQUIT, SIG_DFL);

    return;
}

int main(void) {

    signal(SIGQUIT, sig_quit);

    for (;;);

    return 0;
}