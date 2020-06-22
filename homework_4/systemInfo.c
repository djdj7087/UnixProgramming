/*
 * systemInfo.c -- CP33357 assignment 4, Spring 2020
 * Choi Woo Young, id #201524600, section 059, TAK. SUNG WOO
 *
 * References : https://hyess.tistory.com/461
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <syslog.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utmp.h>

/* Global Variables for log size */
static u_long wtmp_log_size, wtmp_old_log_size;

/**
 * Function for Create Daemon Process.
 */
void make_daemon() {
    pid_t pid;              // variable for process id.

    /*
     * Clear file creation mask
     * The Child Process inherit umask value from its parent
     */
    umask(0);

    /* Become a session leader to lose controlling TTY */
    if ((pid = fork()) < 0)
        exit(-1);
    /* Parent goes away- bye-bye */
    else if (pid != 0)
        exit(0);

    /* Child continues */
    setsid();

    /*
     * Initialize the log file
     */
    openlog("201524600", LOG_CONS, LOG_DAEMON);
}

/**
 * Function for logging
 */
void log_watch_dog() {

    FILE *cse;              // variable for 'cse' file
    FILE *wtmp;             // variable for 'wtmp' file
    char time_buf[40];     // variable for time string
    char log_buf[100];      // variable for log message

    /*
     * Make cse file under the /tmp/homework_4
     * which I made just before in line 35-42
     */
    if ((cse = fopen("cse", "a")) < 0) {
        fprintf(stderr, "log_watch_dog can't open cse file.\n");
        return;
    }

    /*
     * We will chase wtmp file to detect user log.
     * so, we need to open wtmp file.
     */
    if ((wtmp = fopen("/var/log/wtmp", "rb")) < 0) {
        fprintf(stderr, "log_watch_dog can't open wtmp file.\n");
        return;
    }

    /*
     * After Open wtmp file, we need to detect changes in wtmp.
     * So, we need current cursor and some changes in wtmp file.
     */
    // First, we need to move cursor to end of wtmp file.
    fseek(wtmp, 0, SEEK_END);

    // Second, we need to know number of lines of wtmp file.
    wtmp_log_size = ftell(wtmp) / sizeof(struct utmp);

    // Third, we need to define utmp structure
    struct utmp user_info[wtmp_log_size];

    // Finally, we need to move cursor to wtmp file's first
    fseek(wtmp, 0, SEEK_SET);

    /*
     * We set wtmp file and cse file.
     * So, We need to detect wtmp file's changes.
     * if wtmp_log_size is not equal to wtmp_old_log_size, it means wtmp file has change.
     */
    // First, check if wtmp_log_size and wtmp_old_log_size is equal.
    if (wtmp_log_size != wtmp_old_log_size) {
        // Second, if wtmp_log_size and wtmp_old_log_size is not equal, Read all logs in wtmp file
        fread(&user_info, sizeof(struct utmp), wtmp_log_size, wtmp);

        for (int i = wtmp_old_log_size; i < wtmp_log_size; ++i) {
            // Variable for new user_info
            struct utmp new_user_info = user_info[i];

            // Third, we need to know and reformat about user log time.
            time_t current_time_second = (time_t) new_user_info.ut_tv.tv_sec;
            strftime(time_buf, sizeof(struct tm), "%Y년_%m월_%d일/%H시_%M분_%S초", localtime(&current_time_second));

            /*
             * Finally, we need to define user log is 'login' or 'logout'
             * if 'login', utmp's type is USER_PROCESS,
             * else if 'logout', utmp's type is DEAD_PROCESS.
             * I will use these types and write to cse file about user's log information.
             */
            // First, user_info's type is USER_PROCESS.
            if (new_user_info.ut_type == USER_PROCESS) {
                sprintf(log_buf, "USER: %s, LOG_IN TIME: %s\n", new_user_info.ut_user, time_buf);

                // Using fwrite function for writing logs in cse file.
                fwrite(log_buf, strlen(log_buf), 1, cse);
                syslog(LOG_INFO, "%s", log_buf);
            }

            // Second, user_info's type is DEAD_PROCESS.
            if (new_user_info.ut_type == DEAD_PROCESS) {
                sprintf(log_buf, "USER: %s, LOG_OUT TIME: %s\n", new_user_info.ut_user, time_buf);

                // Using fwrite function for writing logs in cse file.
                fwrite(log_buf, strlen(log_buf), 1, cse);
                syslog(LOG_INFO, "%s", log_buf);
            }
        }

        // We finish log change detection, we need to change wtmp_old_log_size to wtmp_log_size.
        wtmp_old_log_size = wtmp_log_size;
    }

    // If we finished file control, we need to close files.
    fclose(wtmp);
    fclose(cse);
}

int main(void) {

    make_daemon();

    /*
     * Change the current working directory to the root
     * so, we will not prevent the file systems from being unmounted
     */
    /* If directory is already exist... remove previous directory and recreate */
    if (rmdir("/tmp/homework_4") < 0) {
        /* If directory has cse file, cannot remove directory, so remove cse file. */
        if (remove("/tmp/homework_4/cse") < 0) {
            fprintf(stderr, "this program cannot remove cse file.\n");
            return 0;
        }
        rmdir("/tmp/homework_4");
    }

    /* First, make new directory */
    if (mkdir("/tmp/homework_4", 0777) < 0)
        fprintf(stderr, "homework_4 directory is already exist.\n");

    /* Second, we move to current working directory */
    if (chdir("/tmp/homework_4") < 0) {
        fprintf(stderr, "log_watch_dog can't change directory to /tmp/homework_4\n");
        return 0;
    }

    while (1) {
        log_watch_dog();

        // We need to sleep 30 Seconds.
        sleep(30);
    }
}