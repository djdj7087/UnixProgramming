//  elapsedtime.c
// Created by wooyoung on 4/4/20.
//

#include "myheader.h"

void *elapsed_time_thread(void *data) {

    /* For hh-mm-ss */
    int hour1 = 0;
    int hour2 = 0;
    int minute1 = 0;
    int minute2 = 0;
    int second1 = 0;
    int second2 = 0;

    while (data) {
        mvprintw(18, 12, "Elapsed Time");
        mvprintw(20, 14, "%d", hour1);
        mvprintw(20, 15, "%d-", hour2);
        mvprintw(20, 17, "%d", minute1);
        mvprintw(20, 18, "%d-", minute2);
        mvprintw(20, 20, "%d", second1);
        mvprintw(20, 21, "%d", second2);

        second2++;
        if (second2 == 10) {
            second1++;
            second2 = 0;
        }
        if (second1 == 6) {
            minute2++;
            second1 = 0;
        }
        if (minute2 == 10) {
            minute1++;
            minute2 = 0;
        }
        if (minute1 == 6) {
            hour2++;
            minute1 = 0;
        }
        if (hour2 == 10) {
            hour1++;
            hour2 = 0;
        }
        if (hour1 == 10) {
            hour1 = 0;
            hour2 = 0;
            minute1 = 0;
            minute2 = 0;
            second1 = 0;
            second2 = 0;
        }
        /* Refresh Screen */
        refresh();
        sleep(1);
    }
}
