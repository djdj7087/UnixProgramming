/*
 * drawbox.c -- CP33357 assignment 3, Spring 2020
 * Choi Woo Young, id #201524600, section 059, TAK. SUNG WOO
 */

#include "mychatheader.h"

void *draw_box()
{
    box(input_scr, ACS_VLINE, ACS_HLINE);
    box(output_scr, ACS_VLINE, ACS_HLINE);
    box(logging_scr, ACS_VLINE, ACS_HLINE);
    box(time_scr, ACS_VLINE, ACS_HLINE);

    wrefresh(input_scr);
    wrefresh(output_scr);
    wrefresh(logging_scr);
    wrefresh(time_scr);

    return NULL;
}