/*
 * cleanup.c -- CP33357 assignment 3, Spring 2020
 * Choi Woo Young, id #201524600, section 059, TAK. SUNG WOO
 */

#include "mychatheader.h"

void cleanup() {
    delwin(input_scr);
    delwin(output_scr);
    endwin();
}