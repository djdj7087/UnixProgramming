//  main.c
// Created by wooyoung on 4/4/20.
//
#include "myheader.h"

/*
 * -------------------------------------
 * For Display Sub Windows For Clock
 * For Create Threads
 * -------------------------------------
 */
int main() {

    /* Declare and initial Clock Sub Windows */
    WINDOW *CurrentLocalDate, *CurrentLocalTime, *ElapsedTime;

    initscr();      // For Screen Initiate.

    /* For Hide Cursor */
    curs_set(FALSE);

    /* Create on the other WINDOW screen */
    CurrentLocalDate =  subwin(stdscr, 8, 40, 0, 0);
    CurrentLocalTime =  subwin(stdscr, 8, 40, 8, 0);
    ElapsedTime =       subwin(stdscr, 8, 40, 16, 0);

    /* Make Box */
    box(CurrentLocalDate, ACS_VLINE, ACS_HLINE);
    box(CurrentLocalTime, ACS_VLINE, ACS_HLINE);
    box(ElapsedTime, ACS_VLINE, ACS_HLINE);

    wrefresh(CurrentLocalDate);
    wrefresh(CurrentLocalTime);
    wrefresh(ElapsedTime);

    /* Thread for three clocks */
    pthread_t   localdate_t, localtime_t;
    int         localdate_return;
    int         localtime_return;

    /* Create Threads */
    // Local Date Thread
    pthread_create(&localdate_t, NULL, local_date_thread, (void *) TRUE);
    // Local Time Thread
    pthread_create(&localtime_t, NULL, local_time_thread, (void *) TRUE);

    /* For Wait to Finish Local Time Thread */
    pthread_join(localtime_t, (void *)&localtime_return);

    /* When want to Exit, We need to */
    delwin(CurrentLocalDate);
    delwin(CurrentLocalTime);
    delwin(ElapsedTime);

    endwin();
    return 0;
}
