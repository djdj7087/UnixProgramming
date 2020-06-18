//
// Created by djdj7 on 2020-06-14.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

    pid_t pid, waitPID;
    int status;
    int data = 10;

    printf("parent process id: %d\n", getpid());

    if ((pid = fork()) < 0)
        perror("fork error");

    if (pid == 0) {     // Child Process
        printf("child process id: %d\n", getpid());
        data = data + 10;
        exit(7);
    } else {            // Parent Process
        data = data - 10;
        sleep(20);
    }

    waitPID = wait(&status);

    if (waitPID != pid)       // Wait for child
        perror("wait error");

    printf("\n waitPID: %d\n", waitPID);

    /* Child $ Parent Process */
    printf("data: %d\n", data);

    return 0;
}