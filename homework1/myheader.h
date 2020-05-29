//
// Created by wooyoung on 4/4/20.
//

#ifndef UNIXHOMEWORK1_HEADER_H
#define UNIXHOMEWORK1_HEADER_H

#include <unistd.h>
#include <ncurses.h>
#include <pthread.h>
#include <time.h>

void *elapsed_time_thread(void *data);
void *local_time_thread(void *data);
void *local_date_thread(void *data);

#endif //UNIXHOMEWORK1_HEADER_H