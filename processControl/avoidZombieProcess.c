//
// Created by djdj7 on 2020-06-14.
//

/**
 * 1st Child Process 가 2nd Child Process fork 하고 나면, terminate 된다.
 * 그렇게 되면 2nd Child Process 가 Orphan Process 가 되어 버리게 된다.
 * 하지만 Parent Process 를 System Daemon 이 대신 역할을 하게 되면서 2nd Child Process 가
 * Orphan Process 가 되었지만, Zombie Process 가 되지 않는 것이다.
 */

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

    /* 1st fork() */
    if ((pid = fork()) < 0)
        perror("fork error");

    if (pid == 0) {     // Child Process
        /* 2nd fork() */
        if ((pid = fork()) < 0)
            perror("fork error");

        /* parent process for 2nd fork() */
        else if (pid > 0) {
            printf("first child, parent id: %d\n", getppid());
            printf("first child, its own id: %d\n", getpid());
            exit(0);
        }

        /* 2nd child process */
        sleep(3);
        printf("second child, parent id: %d\n", getppid());
        printf("second child, its own id: %d\n", getpid());

        exit(0);

    }

    /* pid ==> 1st child process */
    waitPID = waitpid(pid, NULL, 0);

    if (waitPID != pid)       // Wait for child
        perror("wait error");

    printf("parent process, parent id: %d\n", getppid());
    printf("parent process, its own id: %d\n", getpid());

    exit(0);

    return 0;
}