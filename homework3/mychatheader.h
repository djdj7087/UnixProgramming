/*
 * mychatheader.h -- CP33357 assignment 3, Spring 2020
 * Choi Woo Young, id #201524600, section 059, TAK. SUNG WOO
 */

#ifndef HOMEWORK3_MYCHATHEADER_H
#define HOMEWORK3_MYCHATHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define INPUT_WINDOW_H      4       // Input window height
#define INPUT_WINDOW_W      60      // Input window width
#define SHM_RW_FLAG         0666    // Shared memory Read Write Flag
#define EXIT_STATUS         -1
#define BUFFSIZE 1024

int row;                            // terminal max row
int col;                            // terminal max col

typedef struct message {
    long id;                        // message id
    char userID[20];                // message owner's ID
    char message[40];               // message strings
} MESSAGE_INFO;

typedef struct person {
    char userID[20];                // user's ID
} PERSON_INFO;

typedef struct {
    MESSAGE_INFO messageContainer[BUFFSIZE];    // Shared memory for messages
    PERSON_INFO personContainer[BUFFSIZE];      // Shared memory for persons
    int totalMessageCount;                      // Number of Messages
    int totalPersonCount;                       // Number of Persons
} CHAT_INFO;

void *print_time();                 // Function Prototype for print time

void print_chat();                  // Function Prototype for print chat history

void *print_logging();              // Function Prototype for print logging in users

void *get_input();                  // Function Prototype for get input

void *recv_send();                  // Function Prototype for print new message

void *draw_box();                   // Function Prototype for draw and refresh boxes

void chatremove();                  // Function Prototype for erase shared memory

void die(char *msg);                // Function Prototype for exit program

void cleanup();                     // Function Prototype for clean up screens

void chat();                        // Function Prototype for chatting main

WINDOW *input_scr;      // Window for input
WINDOW *output_scr;     // Window for output
WINDOW *logging_scr;    // Window for Logging in Accounts
WINDOW *time_scr;       // Window for elapsed time

struct message buff_in;      // message buffer for input
struct message buff_out;     // message buffer for output

#endif //HOMEWORK3_MYCHATHEADER_H
