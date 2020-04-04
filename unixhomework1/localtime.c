//  localtime.c
// Created by wooyoung on 4/4/20.
//

#include "myheader.h"

void *local_time_thread(void *data) {
    /* Thread Declare for Elapsed Time */
    pthread_t elapsedtime_t;
    int elapsedtime_return;

    // Elapsed Time Thread
    pthread_create(&elapsedtime_t, NULL, elapsed_time_thread, (void *) TRUE);

    while (data) {
        /* Declare Time Variable */
        time_t current_time = time(NULL);
        /* localtime function returns tm structure. */
        struct tm *time_info = localtime(&current_time);

        /* Variables for Current Local time */
        /* Second 0 ~ 59 */
        int second = time_info->tm_sec;
        /* Minute 0 to 59 */
        int minute = time_info->tm_min;
        /* Hour 0 to 23 */
        int hour = time_info->tm_hour;

        mvprintw(10, 8, "Current Local Time");
        if (hour >= 10) {
            mvprintw(12, 12, "%2d-", hour);
        } else {
            mvprintw(12, 12, "0");
            mvprintw(12, 13, "%d-", hour);
        }
        if (minute >= 10) {
            mvprintw(12, 15, "%2d-", minute);
        } else {
            mvprintw(12, 15, "0");
            mvprintw(12, 16, "%d-", minute);
        }
        if (second >= 10) {
            mvprintw(12, 18, "%2d", second);
        } else {
            mvprintw(12, 18, "0");
            mvprintw(12, 19, "%d", second);
        }

        sleep(1);
        refresh();
    }
    /* For Wait to Finish Elapsed Thread */
    pthread_join(elapsedtime_t, (void *) &elapsedtime_return);
    data = false;
}