//
// Created by djdj7 on 2020-06-17.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void daemonize(const char *cmd) {
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    /*
     * Clear file creation mask
     * The Child Process inherit umask value from its parent
     */
    umask(0);

    /* Get the maximum number of file descriptors */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        fprintf(stderr, "%s can't fet week12_file limit", cmd);

    /* Become a session leader to lose controlling TTY */
    if ((pid = fork()) < 0)
        exit(-1);
    /* Parent goes away- bye-bye */
    else if (pid != 0)
        exit(0);

    /* Child continues */

    setsid();

    /* Ignore SIGHANGUP */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        fprintf(stderr, "%s can't ignore SIGHUP", cmd);

    /*
     * Let's do another fork() not to become a zombie Process
     * That's why we create the second child after the first child dies
     */
    if ((pid = fork()) < 0)
        exit(-1);
    else if (pid != 0)
        exit(0);

    /*
     * Change the current working directory to the root
     * so, we will not prevent the file systems from being unmounted
     */
    if (chdir("/") < 0)
        fprintf(stderr, "%s can't change directory to /(root)", cmd);

    /*
     * Ensure future opens will not allocate controlling TTY
     * Close all files
     */
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    /*
     * Attach file descriptors 0, 1, 2 to /dev/null
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /*
     * Initialize the log file
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected week12_file descriptor %d %d %d",
                fd0, fd1, fd2);
        exit(1);
    }
}

int main(void) {

    char sbuf[100] = {'\0'};

    daemonize("myComplicatedDaemon");

    int i = 0;

    while (1) {
        /* Insert your codes as here */
        sprintf(sbuf, "myDaemon: %d seconds\n", i);

        syslog(LOG_INFO, sbuf);

        sleep(2);
        i += 2;
    }

    return 0;
}