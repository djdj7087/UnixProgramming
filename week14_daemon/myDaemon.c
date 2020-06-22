//
// Created by djdj7 on 2020-06-17.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * 절대 끝나지 않는다. Daemon Process.
 * => Daemon Process Keyboard 입력 안먹는다.
 * 끝내려면 kill -9 "pid" 수행.
 */

int daemon_init(void) {

    pid_t pid;

    if ((pid = fork()) < 0)
        return -1;
    /* Parent Goes away- bye-bye */
    else if (pid != 0)
        exit(0);

    /* Child Continues */
    setsid();
    chdir("/");
    /* Clear our file mode creation mask */
    umask(0);

    return 0;
}

int main(void) {

    daemon_init();

    int i = 0;

    while (1) {
        /* Insert your codes as here */
        printf("myDaemon: %d seconds\n", i);
        sleep(2);
        i += 2;
    }

    return 0;
}