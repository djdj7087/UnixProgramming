//
// Created by djdj7 on 2020-06-14.
//

/**
 *  WCOREDUMP -> function pointer.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void pr_exit(int status) {
    if( WIFEXITED(status) )
        printf("normal termination, exit status=%d\n",
               WEXITSTATUS(status));
    else if( WIFSIGNALED(status) )
        printf("abnormal termination, signal number=%d%s\n",
               WTERMSIG(status),
#ifdef WCOREDUMP
               WCOREDUMP(status) ? " (core file generated)" : "");
#else
        "");
#endif

    else if( WIFSTOPPED(status) )
        printf("child stopped, signal number=%d\n",
               WSTOPSIG(status));
}



int main(void) {

    pid_t pid;                      // process id
    pid_t waitPID;
    int status;

    printf("parent process id: %d\n", getpid());

    if ((pid = fork()) < 0)
        perror("fork error");
    else if (pid == 0) {             // Child
        abort();
    }

    printf("\n pid: %d\n", pid);

    waitPID = wait(&status);

    if (waitPID != pid)       // Wait for child
        perror("wait error");

    printf("\n waitPID: %d\n", waitPID);

    /**
     * fork 한번해서 child 가 하나밖에 없어서 waitPID = -1
     * wait error: No child Processes.
     */
    /*
   waitPID = wait(&status);
   printf("\n waitPID: %d\n", waitPID);

   if (waitPID != pid && errno == ECHILD)
       perror("wait error");
   */

    pr_exit(status);

    if ((pid = fork()) < 0)
        perror("fork error");
    else if (pid == 0) {             // Child
        abort();
    }

    printf("\n pid: %d\n", pid);

    waitPID = wait(&status);

    if (waitPID != pid)       // Wait for child
        perror("wait error");

    printf("\n waitPID: %d\n", waitPID);

    pr_exit(status);

    if ((pid = fork()) < 0)
        perror("fork error");
    else if (pid == 0) {             // Child
        /**
         * 여기서 sleep 하면 terminate 되어도 child process 가 끝나지 않음
         * terminate 되어도 ps 명령어를 보면 child process 가 돌아가고 있는 것 확인 할 수 있음.
         * 이런 경우를 orphan(고아) process 라고 한다. (Parent Process 가 terminate 되었기 때문에)
         */
        sleep(10);
        status /= 0;
    }

    printf("\n pid: %d\n", pid);

    waitPID = waitpid(pid, &status, WNOHANG);

    if (waitPID != pid)       // Wait for child
        perror("wait error");
    else if (waitPID == 0)
        perror("Child processes do not finished yet.");

    printf("\n waitPID: %d\n", waitPID);

    pr_exit(status);

    return 0;
}