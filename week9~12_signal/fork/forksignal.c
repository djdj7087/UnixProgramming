#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

/* Set Signal Handler */
void signalHandler(int signal) {

    printf("Catch Signal: %d\n", signal);

    if (signal == SIGCHLD) {
        printf("Child Process is terminated!!\n");
        /* Get the Child Process status when it terminated. */
        wait(NULL);
    }
}

int main() {

    signal(SIGALRM, signalHandler);
    signal(SIGUSR1, signalHandler);
    signal(SIGCHLD, signalHandler);

    /* !fork() means fork() == 0 */
    if (!fork()) {

        /* Child Process Running */
        printf("Child Process is Running....\n");
        sleep(2);
        printf("Child sending SIGALRM!!\n");
        /* Send alarm week9~12_signal to parent */
        kill(getppid(), SIGALRM);
        sleep(5);
        printf("Child is exiting...\n");
        return 0;
    }

    printf("Parent Process is Running, PID = %d\n", getpid());
    printf("Press ENTER to EXIT...\n");
    getchar();
    printf("Parent is terminated!!\n");

    return 0;
}