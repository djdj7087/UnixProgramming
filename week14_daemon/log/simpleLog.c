//
// Created by djdj7 on 2020-06-17.
//

#include <syslog.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char sbuf[100] = {'\0'};

    int fd0, fd1, fd2;

    /*
     * Attach week12_file descriptors 0, 1, 2 to /dev/null
     */
    close(0);
    close(1);
    close(2);

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    sprintf(sbuf, "fd0: %d, fd1: %d, fd2: %d\n",
            fd0, fd1, fd2);

    printf("fd0: %d\n", fd0);
    printf("fd1: %d\n", fd1);
    printf("fd2: %d\n", fd2);

    /* Log Text Area */
    openlog("Logs", LOG_PID, LOG_USER);

    syslog(LOG_INFO, sbuf);

    closelog();

    return 0;
}