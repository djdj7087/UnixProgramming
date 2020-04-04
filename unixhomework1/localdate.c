//  localdate.c
// Created by wooyoung on 4/4/20.
//

#include "myheader.h"

void *local_date_thread(void *data) {
    /* Array for store DAY_OF_THE_WEEK Strings */
    char* day_of_week[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    /* Declare Time Variable */
    time_t current_time = time(NULL);
    /* localtime function returns tm structure. */
    struct tm *time_info = localtime(&current_time);

    /* Variables for Current Local Date */
    /* year from 1900 */
    int year = time_info->tm_year + 1900;
    /* month 0 ~ 11 */
    int month = time_info->tm_mon + 1;
    /* month_day 1 ~ 31(30/28/29) */
    int month_day = time_info->tm_mday;
    /* week_day 0(SUN) to 6(SAT) */
    int week_day = time_info->tm_wday;

    /* Print Screen */
    mvprintw(2, 8, "Current Local Date");
    mvprintw(4, 10, "%4d-", year);
    if (month >= 10) {
        mvprintw(4, 15, "%2d-", month);
    } else {
        mvprintw(4, 15, "0");
        mvprintw(4, 16, "%d-", month);
    }
    if (month_day >= 10) {
        mvprintw(4, 18, "%2d-", month_day);
    } else {
        mvprintw(4, 18, "0");
        mvprintw(4, 19, "%d", month_day);
    }
    mvprintw(4, 21, "%s", day_of_week[week_day]);
}